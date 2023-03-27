import sys
import queue
import random

qsize = int(sys.argv[1])   # size of queue
interval = int(sys.argv[2])      # time interval between arrival of 2 quests
CPU_num = int(sys.argv[3])    # number of CPU
deal_time = int(sys.argv[4])  # average time needed for each quest

interval_low = int(interval-interval/4)
interval_high = int(interval+interval/4)
total_quest = 0
rejected_quest = 0
sum_size = 0
response_time = 0
dealt_quest = 0
wait = 0

que = queue.Queue(qsize)
CPUArray = [0]*CPU_num

for t in range(1000000):
  # print(t)
  if wait == 0:
    # next quest is ready
    total_quest += 1
    # and see if it can be put into list
    if not que.full():
      que.put(t)
    else:
      rejected_quest += 1
    wait = random.randint(interval_low, interval_high)
  else:
    wait -= 1

  # busy CPU deal with quest
  for i in range(CPU_num):
    if CPUArray[i] > 0:
      CPUArray[i] -= 1

  # only one idle CPU accept new quest
  for i in range(CPU_num):
    if CPUArray[i] == 0 and que.empty() == False:
      # add up response time
      response_time += t - que.get()
      CPUArray[i] = deal_time
      # count num of dealt quest
      dealt_quest += 1
      break

  # get current qsize
  sum_size += que.qsize()

print('Percentage of requests that was rejected: ', rejected_quest*1.0/total_quest)
print('Average queue size: ', sum_size*1.0/1000000)
print('Average response time', response_time*1.0/dealt_quest)
