#include "mbed.h"
#include "uLCD_4DGL.h"
#include "mbed_rpc.h"
#include "config.h"
#include "magic_wand_model_data.h"
#include "accelerometer_handler.h"
#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/micro/kernels/micro_ops.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"
#include "MQTTNetwork.h"
#include "MQTTmbed.h"
#include "MQTTClient.h"
#include "stm32l475e_iot01_accelero.h"
#include "math.h"
using namespace std::chrono;
uLCD_4DGL uLCD(D1, D0, D2);
DigitalOut led3(LED3);
DigitalOut led2(LED2);
Thread TFthread(osPriorityNormal, 8 * 1024);
void TF();
Thread Tiltthread;
bool mode_G = 0;
bool mode_T = 0;
int16_t PDataXYZ[3] = {0};
int16_t gDataXYZ[3] = {0};
int angle = 30;
float cur = 0;
int num = 0; // event number
void mode_c(Arguments *in, Reply *out);
BufferedSerial pc(USBTX, USBRX);
RPCFunction rpcLoop(&mode_c, "mode_c");
constexpr int kTensorArenaSize = 60 * 1024;
uint8_t tensor_arena[kTensorArenaSize];
WiFiInterface *wifi;
InterruptIn but1(USER_BUTTON);
volatile int message_num = 0;
volatile int arrivedcount = 0;
volatile bool closed = false;
const char *topic = "Mbed";
Thread mqtt_thread(osPriorityHigh);
Thread WIFI_MQTT(osPriorityHigh);
EventQueue mqtt_queue;

//////////////////////////////////////////////

int PredictGesture(float *output)
{
	// How many times the most recent gesture has been matched in a row
	static int continuous_count = 0;
	// The result of the last prediction
	static int last_predict = -1;
	// Find whichever output has a probability > 0.8 (they sum to 1)
	int this_predict = -1;
	for (int i = 0; i < label_num; i++)
	{
		if (output[i] > 0.8)
			this_predict = i;
	}
	// No gesture was detected above the threshold
	if (this_predict == -1)
	{
		continuous_count = 0;
		last_predict = label_num;
		return label_num;
	}
	if (last_predict == this_predict)
	{
		continuous_count += 1;
	}
	else
	{
		continuous_count = 0;
	}
	last_predict = this_predict;
	// If we haven't yet had enough consecutive matches for this gesture,
	// report a negative result
	if (continuous_count < config.consecutiveInferenceThresholds[this_predict])
	{
		return label_num;
	}
	// Otherwise, we've seen a positive result, so clear all our variables
	// and report it
	continuous_count = 0;
	last_predict = -1;
	return this_predict;
}
void messageArrived(MQTT::MessageData &md)
{
	MQTT::Message &message = md.message;
	char msg[300];
	sprintf(msg, "Message arrived: QoS%d, retained %d, dup %d, packetID %d\r\n", message.qos, message.retained, message.dup, message.id);
	printf(msg);
	ThisThread::sleep_for(1000ms);
	char payload[300];
	sprintf(payload, "Payload %.*s\r\n", message.payloadlen, (char *)message.payload);
	printf(payload);
	++arrivedcount;
}
void publish_message(MQTT::Client<MQTTNetwork, Countdown> *client)
{
	message_num++;
	MQTT::Message message;
	char buff[100];
	sprintf(buff, "Angle threshold is %d", angle);
	message.qos = MQTT::QOS0;
	message.retained = false;
	message.dup = false;
	message.payload = (void *)buff;
	message.payloadlen = strlen(buff) + 1;
	int rc = client->publish(topic, message);
	printf("rc:  %d\r\n", rc);
	printf("Publish message: %s\r\n", buff);
}
void close_mqtt()
{
	closed = true;
}
void Tilt(MQTT::Client<MQTTNetwork, Countdown> *client)
{

	BSP_ACCELERO_Init();
	BSP_ACCELERO_AccGetXYZ(gDataXYZ);
	int Axis[3];
	for (int i = 0; i < 3; i++)
	{
		Axis[i] = gDataXYZ[i];
	}

	while (1)
	{
		if (mode_T)
		{
			BSP_ACCELERO_Init();
			BSP_ACCELERO_AccGetXYZ(gDataXYZ);
			led2 = 1;
			long int dotproduct = 0;
			long int normA = 0;
			long int normg = 0;
			for (int i = 0; i < 3; i++)
			{
				dotproduct += gDataXYZ[i] * Axis[i];
				normA += Axis[i] * Axis[i];
				normg += gDataXYZ[i] * gDataXYZ[i];
			}
			float cosvalue = dotproduct / sqrt(normg) / sqrt(normA);
			cur = acos(cosvalue) * 180 / 3.1415926;
			uLCD.locate(1, 2);
			uLCD.printf("Angle Selected: \n %d", angle);
			uLCD.locate(1, 6);
			uLCD.printf("Current angle : \n %.3f", cur);
			ThisThread::sleep_for(250ms);
			if (cur >= angle && num < 5)
			{
				num++;

				MQTT::Message message;
				char buff[100];
				sprintf(buff, "The %d time(s), angle is %f", num, cur);
				message.qos = MQTT::QOS0;
				message.retained = false;
				message.dup = false;
				message.payload = (void *)buff;
				message.payloadlen = strlen(buff) + 1;
				int rc = client->publish(topic, message);
				printf("rc:  %d\r\n", rc);
				printf("Publish message: %s\r\n", buff);
				if (num >= 5)
				{
					mode_T = 0;
				}
			}
		}
		else
		{
			led2 = 0;
		}
	}
}
void wifi_m()
{
	wifi = WiFiInterface::get_default_instance();
	if (!wifi)
	{
		printf("ERROR: No WiFiInterface found.\r\n");
		return -1;
	}
	printf("\nConnecting to %s...\r\n", MBED_CONF_APP_WIFI_SSID);
	int ret = wifi->connect(MBED_CONF_APP_WIFI_SSID, MBED_CONF_APP_WIFI_PASSWORD, NSAPI_SECURITY_WPA_WPA2);
	if (ret != 0)
	{
		printf("\nConnection error: %d\r\n", ret);
		return -1;
	}
	NetworkInterface *net = wifi;
	MQTTNetwork mqttNetwork(net);
	MQTT::Client<MQTTNetwork, Countdown> client(mqttNetwork);
	//TODO: revise host to your IP
	const char *host = "172.20.10.4";
	printf("Connecting to TCP network...\r\n");
	SocketAddress sockAddr;
	sockAddr.set_ip_address(host);
	sockAddr.set_port(1883);
	printf("address is %s/%d\r\n", (sockAddr.get_ip_address() ? sockAddr.get_ip_address() : "None"), (sockAddr.get_port() ? sockAddr.get_port() : 0)); //check setting
	int rc = mqttNetwork.connect(sockAddr);																											   //(host, 1883);
	if (rc != 0)
	{
		printf("Connection error.");
		return -1;
	}
	printf("Successfully connected!\r\n");
	MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
	data.MQTTVersion = 3;
	data.clientID.cstring = "Mbed";
	if ((rc = client.connect(data)) != 0)
	{
		printf("Fail to connect MQTT\r\n");
	}
	if (client.subscribe(topic, MQTT::QOS0, messageArrived) != 0)
	{
		printf("Fail to subscribe\r\n");
	}
	mqtt_thread.start(callback(&mqtt_queue, &EventQueue::dispatch_forever));
	but1.rise(mqtt_queue.event(&publish_message, &client));
	Tiltthread.start(callback(&Tilt, &client));
	int num = 0;
	while (num != 5)
	{
		client.yield(100);
		++num;
	}
	while (1)
	{
		if (closed)
			break;
		client.yield(500);
		ThisThread::sleep_for(500ms);
	}
	printf("Ready to close MQTT Network......\n");

	if ((rc = client.unsubscribe(topic)) != 0)
	{
		printf("Failed: rc from unsubscribe was %d\n", rc);
	}
	if ((rc = client.disconnect()) != 0)
	{
		printf("Failed: rc from disconnect was %d\n", rc);
	}

	mqttNetwork.disconnect();
	printf("Successfully closed!\n");
}
int main()
{
	TFthread.start(TF);
	BSP_ACCELERO_Init();
	char buf[256], outbuf[256];
	FILE *devin = fdopen(&pc, "r");
	FILE *devout = fdopen(&pc, "w");

	uLCD.cls();
	uLCD.textbackground_color(0x0);
	uLCD.background_color(0x0);
	uLCD.printf("\n108060033\n");
	uLCD.text_width(1);
	uLCD.text_height(1);

	WIFI_MQTT.start(&wifi_m);
	while (true)
	{
		memset(buf, 0, 256); // clear buffer
		for (int i = 0; i < 255; i++)
		{
			char recv = fgetc(devin);
			if (recv == '\r' || recv == '\n')
			{
				printf("\r\n");
				break;
			}
			buf[i] = fputc(recv, devout);
		}
		RPC::call(buf, outbuf);
		printf("%s\r\n", outbuf);
	}
	return 0;
}

void TF()
{
	led3 = 1;
	bool should_clear_buffer = false;
	bool got_data = false;
	int gesture_index;
	static tflite::MicroErrorReporter micro_error_reporter;
	tflite::ErrorReporter *error_reporter = &micro_error_reporter;
	const tflite::Model *model = tflite::GetModel(g_magic_wand_model_data);

	if (model->version() != TFLITE_SCHEMA_VERSION)
	{
		error_reporter->Report(
			"Model provided is schema version %d not equal "
			"to supported version %d.",
			model->version(), TFLITE_SCHEMA_VERSION);
		return;
	}
	static tflite::MicroOpResolver<6> micro_op_resolver;
	micro_op_resolver.AddBuiltin(
		tflite::BuiltinOperator_DEPTHWISE_CONV_2D,
		tflite::ops::micro::Register_DEPTHWISE_CONV_2D());
	micro_op_resolver.AddBuiltin(tflite::BuiltinOperator_MAX_POOL_2D,
								 tflite::ops::micro::Register_MAX_POOL_2D());
	micro_op_resolver.AddBuiltin(tflite::BuiltinOperator_CONV_2D,
								 tflite::ops::micro::Register_CONV_2D());
	micro_op_resolver.AddBuiltin(tflite::BuiltinOperator_FULLY_CONNECTED,
								 tflite::ops::micro::Register_FULLY_CONNECTED());
	micro_op_resolver.AddBuiltin(tflite::BuiltinOperator_SOFTMAX,
								 tflite::ops::micro::Register_SOFTMAX());
	micro_op_resolver.AddBuiltin(tflite::BuiltinOperator_RESHAPE,
								 tflite::ops::micro::Register_RESHAPE(), 1);
	// Build an interpreter to run the model with
	static tflite::MicroInterpreter static_interpreter(
		model, micro_op_resolver, tensor_arena, kTensorArenaSize, error_reporter);
	tflite::MicroInterpreter *interpreter = &static_interpreter;
	// Allocate memory from the tensor_arena for the model's tensors
	interpreter->AllocateTensors();
	// Obtain pointer to the model's input tensor
	TfLiteTensor *model_input = interpreter->input(0);
	if ((model_input->dims->size != 4) || (model_input->dims->data[0] != 1) ||
		(model_input->dims->data[1] != config.seq_length) ||
		(model_input->dims->data[2] != kChannelNumber) ||
		(model_input->type != kTfLiteFloat32))
	{
		error_reporter->Report("Bad input tensor parameters in model");
		return;
	}
	int input_length = model_input->bytes / sizeof(float);
	TfLiteStatus setup_status = SetupAccelerometer(error_reporter);
	if (setup_status != kTfLiteOk)
	{
		error_reporter->Report("Set up failed\n");
		return;
	}
	error_reporter->Report("Set up successful...\n");
	while (1)
	{
		if (mode_G)
		{
			led3 = 1;
			// Attempt to read new data from the accelerometer
			got_data = ReadAccelerometer(error_reporter, model_input->data.f,
										 input_length, should_clear_buffer);
			// If there was no new data,
			// don't try to clear the buffer again and wait until next time
			if (!got_data)
			{
				should_clear_buffer = false;
				continue;
			}
			// Run inference, and report any error
			TfLiteStatus invoke_status = interpreter->Invoke();
			if (invoke_status != kTfLiteOk)
			{
				error_reporter->Report("Invoke failed on index: %d\n", begin_index);
				continue;
			}
			// Analyze the results to obtain a prediction
			gesture_index = PredictGesture(interpreter->output(0)->data.f);
			// Clear the buffer next time we read data
			should_clear_buffer = gesture_index < label_num;
			// Produce an output
			if (gesture_index < label_num)
			{
				if (angle <= 85)
					angle += 5;
				else
					angle = 0;
				uLCD.locate(1, 2);
				uLCD.printf("Angle Selected: \n %d", angle);
				uLCD.locate(1, 6);
				uLCD.printf("Tilt angle : \n x        ");
				error_reporter->Report(config.output_message[gesture_index]);
			}
		}
		else
		{
			led3 = 0;
		}
	}
}
void mode_c(Arguments *in, Reply *out)
{
	int mode = in->getArg<int>();
	char buffer[200];
	if (mode == 1)
	{
		mode_G = 1;
		mode_T = 0;
		num = 0;
		sprintf(buffer, "Gesture");
	}
	else if (mode == 2)
	{
		mode_G = 0;
		mode_T = 1;
		num = 0;
		sprintf(buffer, "Tilt Detection");
	}
	else if (mode == 3)
	{
		mode_G = 0;
		mode_T = 0;
		num = 0;
		sprintf(buffer, "Back RPC");
	}
	else
	{
		mode_G = 0;
		mode_T = 0;
		num = 0;
		printf("\n");
		SCB->AIRCR = 0x05fa0004;
		sprintf(buffer, "ERROR");
	}
	out->putData(buffer);
}