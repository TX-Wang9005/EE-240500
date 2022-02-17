# Part. 1

#=======================================

# Import module

#  csv -- fileIO operation

import csv

#=======================================


# Part. 2

#=======================================

# Read cwb weather data

cwb_filename = '108060033.csv'

data = []

header = []

with open(cwb_filename) as csvfile:

   mycsv = csv.DictReader(csvfile)

   header = mycsv.fieldnames

   for row in mycsv:

      data.append(row)

#=======================================


# Part. 3

#=======================================

# Analyze data depend on your group and store it to target_data like:

# Retrive all data points which station id is "C0X260" as a list.

target_data1 = list(filter(lambda item: item['station_id'] == 'C0A880', data))
target_data2 = list(filter(lambda item: item['station_id'] == 'C0F9A0', data))
target_data3 = list(filter(lambda item: item['station_id'] == 'C0G640', data))
target_data4 = list(filter(lambda item: item['station_id'] == 'C0R190', data))
target_data5 = list(filter(lambda item: item['station_id'] == 'C0X260', data))
answer = []
answer1 = 0
answer2 = 0
answer3 = 0
answer4 = 0
answer5 = 0
for i in range(len(target_data1)):
    if target_data1[i]['PRES'] != -99.00 or target_data1[i]['PRES'] != -999.000 :
        answer1 = answer1 + float(target_data1[i]['PRES'])
for i in range(len(target_data2)):
    if target_data2[i]['PRES'] != -99.00 or target_data2[i]['PRES'] != -999.000 :
        answer2 = answer2 + float(target_data2[i]['PRES'])

for i in range(len(target_data3)):
    if target_data3[i]['PRES'] != -99.00 or target_data3[i]['PRES'] != -999.000 :
        answer3 = answer3 + float(target_data3[i]['PRES'])

for i in range(len(target_data4)):
    if target_data4[i]['PRES'] != -99.00 or target_data4[i]['PRES'] != -999.000 :
        answer4 = answer4 + float(target_data4[i]['PRES'])

for i in range(len(target_data5)):
    if target_data5[i]['PRES'] != -99.00 or target_data5[i]['PRES'] != -999.000 :
        answer5 = answer5 + float(target_data5[i]['PRES'])


# Retrive ten data points from the beginning.

if answer1 == 0 :
    answer.append(['C0A880', 'none'])
else :
    answer.append(['C0A880', str('%.2f'%(answer1/len(target_data1)))])

if answer2 == 0 :
    answer.append(['C0F9A0', 'none'])
else :
    answer.append(['C0F9A0', str('%.2f'%(answer2/len(target_data2)))])

if answer3 == 0 :
    answer.append(['C0G640', 'none'])
else :
    answer.append(['C0G640', str('%.2f'%(answer3/len(target_data3)))])

if answer4 == 0 :
    answer.append(['C0R190', 'none'])
else :
    answer.append(['C0R190', str('%.2f'%(answer4/len(target_data4)))])

if answer5 == 0 :
    answer.append(['C0X260', 'none'])
else :
    answer.append(['C0X260', str('%.2f'%(answer5/len(target_data5)))])




#=======================================


# Part. 4

#=======================================

# Print result

print(answer)

#======================================== 