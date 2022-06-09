a_global = 42;

def sumup(n):
    result = 0
    n += 1
    for i in range(1, n): #range return 1,2,...,n-1.
      result += i
    return result
