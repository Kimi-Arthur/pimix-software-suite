import csv
import shutil
import os

import glob
def get_actual_filename(name):
    name = "%s[%s]" % (name[:-1], name[-1])
    return glob.glob(name)[0]

dest_dir = os.environ['BIN_DIR']
print(dest_dir)

with open('result.csv', newline='') as f:
    reader = csv.reader(f)
    for row in reader:
        if row[1][1] == ':' and not row[1].startswith("c:\\windows"):
            path = get_actual_filename(row[1])
            print(path)
            try:
                shutil.copy(path, dest_dir)
            except:
                pass
