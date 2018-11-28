import sys
from os import walk
from os.path import join as pathjoin

def main():
  path = 'src'

  cpps = []
  for (p, _, fs) in walk(path):
    for f in fs:
      if f.endswith('.cpp') and 'Test' not in f and 'main' not in f:
        cpps.append(pathjoin(p, f))

  cpps = set(cpps)

  cmake_cpps = []
  with open('CMakeLists.txt') as f:
    it = iter(f)
    for l in it:
      if 'set(SRC_FILES' in l:
        break
    else:
      print("'set(SRC_FILES' not found")
      sys.exit(0)
    for l in it:
      if ')' in l:
        break
      else:
        cmake_cpps.append(l.strip())
  
  cmake_cpps = set(cmake_cpps)

  diff = cpps - cmake_cpps
  if diff:
    # print(cpps)
    # print(cmake_cpps)
    print(diff)
  else:
    print('cmake sources look good')
    

if __name__ == '__main__':
  main()