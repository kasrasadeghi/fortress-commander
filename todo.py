def main():
  with open('docs/todo.md') as f:
    lines = f.readlines()
  
  sections = [{'#': 'root', 'c': []}]
  for l in map(lambda s: s[:-1], lines):
    if l.startswith('#'):
      sections.append({'#': l, 'c': []})
    else:
      sections[-1]['c'].append(l)
  
  from pprint import pprint
  for s in sections[1:]:
    if s['#'].endswith('> done'):
      continue
    print(s['#'])

    todo = []

    done = True
    for l in s['c']:
      if '>- todo' in l:
        done = False
        continue
      else:
        if not done:
          todo.append(l.replace('>-', '').replace('- ', ''))
    
    if todo:
      print("\n".join(map(lambda l: "  " + l, todo)))


if __name__ == '__main__':
  main()