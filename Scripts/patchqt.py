#!/bin/python
import sys
import os.path
import shutil
import json

start_comment = '# Start for patchqt.py'
end_comment = '# End for patchqt.py'

cpp11_config = '''
CONFIG                 += c++11

'''

proj_config = '''
BASE_DIR                = {proj_dir}
SRC_DIR                 = $$BASE_DIR
BIN_DIR                 = $$BASE_DIR/bin
LIB_DIR                 = /usr/lib/pimix

'''

def patch_mkspec(path, proj=None):
    op = path + '.old'
    shutil.move(path, op)
    f = open(op)
    nf = open(path, 'w')

    need_output = True
    for line in f:
        if line.rstrip() == start_comment:
            print('\tConfig already exists!\n\tReplace in place!')
            need_output = False

        if need_output:
            nf.write(line)
        
        if not need_output and line.rstrip() == end_comment:
            need_output = True

    nf.write('\n')
    nf.write(start_comment + '\n')

    nf.write(cpp11_config)
    
    if proj is not None:
        nf.write(proj_config.format(proj_dir=proj))

    nf.write(end_comment + '\n')
    f.close()
    nf.close()

def main():
    if len(sys.argv) < 2:
        print('\tPlease specify the path containing mkspecs folder!')
        return 1

    qt_path = sys.argv[1]

    su = qt_path + '/mkspecs/common/shell-unix.conf'
    sw = qt_path + '/mkspecs/common/shell-win32.conf'

    if not os.path.exists(su) or not os.path.exists(sw):
        print('\tThe mkspecs not exist!\n\tPlease specify the path containing mkspecs folder')
        return 1

    if len(sys.argv) >= 3:
        proj = sys.argv[2]
        return patch_mkspec(su, proj) or patch_mkspec(sw, proj)
    return patch_mkspec(su) or patch_mkspec(sw)

if __name__ == "__main__":
    main()
