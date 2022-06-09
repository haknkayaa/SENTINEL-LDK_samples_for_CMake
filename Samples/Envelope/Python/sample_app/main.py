import moduleA
import moduleB

import time
import sys

print("main.py: Hello, World!\n")

print("main.py: using python: " + str(sys.version) + "\n")

print("main.py: moduleA.a_global: " + str(moduleA.a_global))
print("main.py: moduleB.a_global: " + str(moduleB.a_global))

sumA = moduleA.sumup(100)
print( "main.py: sumA: " + str(sumA) )

print("")

sumB = moduleB.sumup(101)
print( "main.py: sumB: " + str(sumB) )

print("\nSleeping for 10 seconds...")
for i in range(0, 10):
  print( "  i: " + str(i) )
  time.sleep( 1 )

print( "Done." )
