import moduleA
import moduleC

a_global = 666;

def sumup(n):
  print("moduleB.py:sumup: calling moduleC.date")
  moduleC.date()
  print("moduleB.py:sumup: calling moduleA.sumup")
  return moduleA.sumup(n)
