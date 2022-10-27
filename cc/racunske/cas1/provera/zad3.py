def obrni(x):
  y = x.split()
  y.reverse()
  return " ".join(y)
  
  
  
test = input("Enter a sentence: ")
print(obrni(test))
