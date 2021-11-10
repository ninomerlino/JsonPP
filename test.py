import json
data = None
with open("file.json", "r") as file:
    data = json.loads(file.read())
data.append("This json was modfied successfuly")
with open("filedump.json", "w") as file:
    file.write(json.dumps(data))
