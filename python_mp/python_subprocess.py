import subprocess

p1 = subprocess.Popen(['sortnums.out', 'data1.txt', 'sorted1.txt'])
p2 = subprocess.Popen(['sortnums.out', 'data2.txt', 'sorted2.txt'])
p1.wait()
p2.wait()
p3 = subprocess.Popen(['mergenums.out', 'sorted1.txt', 'sorted2.txt', 'merged.txt'])