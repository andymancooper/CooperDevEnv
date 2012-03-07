# File: chaos.py (from chapter 1)
# A simple program illustrating chaotic behavior

# def main():
#     print("This program illustrates a chaotic function")
#     x = eval(input("Enter a number between 0 and 1: "))
#     n = eval(input("How many numbers should I print?"))
#     for i in range(n):
#         x = 3.9 * x * (1 - x)
#         print(x)
# 
# main()
# 

def collectInputs(count = 2):
    result = [ ]
    for i in range(count):
        x = eval(input("Enter a number between 0 and 1: "))
        n = eval(input("How many numbers should I print?"))
        result.append({ 'starting_val': x, 'count': n })
    return result

def doChaos(inputs):
    result = [ ]
    lastVal = inputs['starting_val']
    for i in range(inputs['count']):
        lastVal = 3.9 * lastVal * (1 - lastVal)
        result.append(lastVal)
    return result

inputs = collectInputs()
outputs = [ ]
maxLen = 0
for i in inputs:
    outputs.append(doChaos(i))
    if (i['count'] > maxLen):
        maxLen = i['count']

for j in range(maxLen):
   row = ""
   for output in outputs:
       if (len(output) > j):
           row += " " + str(output[j])
       else:
           row += " n/a"
   print(row)

