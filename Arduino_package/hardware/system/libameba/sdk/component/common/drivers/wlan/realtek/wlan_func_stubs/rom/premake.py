import os
import json
import sys
import posixpath

exclude = [];
exclude_path = [];
target_exclude_path = [];
target_ccflags = [];
target_exclude = [];

files_c = [];
files_cpp = [];
files_asm = [];
files_a = [];
files_h = {};
ccflags = [];
includes = [];
obj_list = [];
inc_path = [];
local_inc_path = [];

premake_python_name = 'premake.py'
config_jason_name = 'config.json'

target = 'rtl8195b-lp'
chip_ver = 'Cut-A'
build_mod = 'All'
root_make_path = ''
toolchain = ''
secure_mode = ''

is_out_lib='N'
lib_outputf='build.a'

cwd=os.path.dirname(os.path.realpath(__file__))
jsonfile = open(os.path.join(cwd,config_jason_name)) 
ocfg = json.load(jsonfile)
jsonfile.close()

if 'IsLib' in ocfg:
    is_out_lib = ocfg['IsLib']

if 'OutLib' in ocfg:
    lib_outputf = ocfg['OutLib']

if 'excludePath' in ocfg:
    exclude_path = ocfg['excludePath']

if 'excludeSource' in ocfg:
    exclude = ocfg['excludeSource']
    
if 'extraCCFlags' in ocfg:
    ccflags = ocfg['extraCCFlags']

if 'extraIncludes' in ocfg:
    includes = ocfg['extraIncludes']

if 'localIncludePath' in ocfg:
    local_inc_path = ocfg['localIncludePath']

cwd=os.path.dirname(os.path.realpath(__file__))

if len(sys.argv) >= 5:
    target = sys.argv[1]
    build_mod = sys.argv[3]
    root_make_path = sys.argv[4]
if len(sys.argv) >= 6:
    toolchain = sys.argv[5]

if len(sys.argv) >= 7:
    secure_mode = sys.argv[6]

if os.path.isfile(os.path.join(root_make_path,'include.txt')):
    with open(root_make_path+'/include.txt') as inc_f:
        inc_path = inc_f.read().splitlines()

#for i in list(inc_path):
#    rel_inc_path = os.path.relpath(i, cwd)
#    print (rel_inc_path)
    
build_mod = build_mod.upper()
if build_mod == 'RAM':
    exclude_path.append('rom')

if target in ocfg:
    if 'excludePath' in ocfg[target]:
        target_exclude_path = ocfg[target]['excludePath']
    if 'CCFlags' in ocfg[target]:
        target_ccflags = ocfg[target]['CCFlags']
    if 'excludeSource' in ocfg[target]:
        target_exclude = ocfg[target]['excludeSource']

if toolchain in ocfg:
    if 'excludePath' in ocfg[toolchain]:
        for i in list(ocfg[toolchain]['excludePath']):
            target_exclude_path.append(i)
        
    if 'CCFlags' in ocfg[toolchain]:
        for i in list(ocfg[toolchain]['CCFlags']):
            target_ccflags.append(i)

    if 'excludeSource' in ocfg[toolchain]:
        for i in list(ocfg[toolchain]['excludeSource']):
            target_exclude.append(i)

    if target in ocfg[toolchain]:
        if 'excludePath' in ocfg[toolchain][target]:
            for i in list(ocfg[toolchain][target]['excludePath']):
                target_exclude_path.append(i)

        if 'excludeSource' in ocfg[toolchain][target]:
            for i in list(ocfg[toolchain][target]['excludeSource']):
                target_exclude.append(i)
if secure_mode == 'Secure' and 'BuildSecureDep' in ocfg:
    if 'excludeSource' in ocfg['BuildSecureDep']:
        for i in list(ocfg['BuildSecureDep']['excludeSource']):
            target_exclude.append(i)
    
if secure_mode == 'NonSecure' and 'BuildNonSecureDep' in ocfg:
    if 'excludeSource' in ocfg['BuildNonSecureDep']:
        for i in list(ocfg['BuildNonSecureDep']['excludeSource']):
            target_exclude.append(i)

makefile_path = os.path.dirname(__file__)

with open(os.path.join(cwd,'Makefile'),'w') as out:
    for root, subFolders, files in os.walk(cwd):
        if os.path.basename(root) in exclude_path or \
            os.path.basename(root) in target_exclude_path: 
            for i in range(len(subFolders)):
                del subFolders[0]
            continue

        if os.path.relpath(root,cwd) != '.':
            if premake_python_name in files and config_jason_name in files:
                for i in range(len(subFolders)):
                    del subFolders[0]
                continue

        path_excluded = 0
        for i in exclude_path:
            if '/' in i:
                if i in os.path.relpath(root):
                    path_excluded = 1
                    for j in range(len(subFolders)):
                        del subFolders[0]
                    break   # break this for loop
        if path_excluded == 1:
            continue

        for i in target_exclude_path:
            if '/' in i:
                if i in os.path.relpath(root):
                    path_excluded = 1
                    for j in range(len(subFolders)):
                        del subFolders[0]
                    break   # break this for loop
        if path_excluded == 1:
            continue

        for filename in files:
#           print(filename)
            if filename in list(exclude):
                continue

            if filename in list(target_exclude):
                continue

            fname,fext=os.path.splitext(filename)
            if fext=='.c' or fext=='.C':
                if not os.path.basename(root) in exclude_path:
                    file_path = os.path.join(root,filename)
                    file_rel_path = os.path.relpath(file_path, makefile_path)
#                    files_c.append(os.path.join(root,filename))
                    print(file_rel_path)
                    files_c.append(file_rel_path)
                    obj_list.append(filename.replace('.c', '.o'))
            elif fext=='.cpp':
                files_cpp.append(os.path.join(root,filename))
            elif fext=='.a':
                files_a.append(os.path.join(root,filename))
#           elif fext=='.s' or fext=='.S':
#               if (fname+'.o') not in obj_list and not os.path.isfile(os.path.join(root,fname+'.c')) and not os.path.isfile(os.path.join(root,fname+'.C')):
#                   files_asm.append(os.path.join(root,filename))
#                   obj_list.append(filename.replace('.s', '.o'))
            elif fext=='.h' or fext=='.hpp':
                files_h[root]=1

    for root, subFolders, files in os.walk(cwd):
        if os.path.basename(root) in exclude_path or \
            os.path.basename(root) in target_exclude_path: 
            for i in range(len(subFolders)):
                del subFolders[0]
            continue

        if os.path.relpath(root,cwd) != '.':
            if premake_python_name in files and config_jason_name in files:
                for i in range(len(subFolders)):
                    del subFolders[0]
                continue

        path_excluded = 0
        for i in exclude_path:
            if '/' in i:
                if i in os.path.relpath(root):
                    path_excluded = 1
                    for j in range(len(subFolders)):
                        del subFolders[0]
                    break   # break this for loop
        if path_excluded == 1:
            continue

        for i in target_exclude_path:
            if '/' in i:
                if i in os.path.relpath(root):
                    path_excluded = 1
                    for j in range(len(subFolders)):
                        del subFolders[0]
                    break   # break this for loop
        if path_excluded == 1:
            continue

        for filename in files:
            if filename in list(exclude):
                continue

            if filename in list(target_exclude):
                continue
                
            fname,fext=os.path.splitext(filename)
            if fext=='.s':
                if filename.replace('.s', '.o') not in obj_list:
                    file_path = os.path.join(root,filename)
                    file_rel_path = os.path.relpath(file_path, makefile_path)
                    print(file_rel_path)
                    files_asm.append(file_rel_path)
#                    files_asm.append(os.path.join(root,filename))
                    obj_list.append(filename.replace('.s', '.o'))

            if fext=='.S':
                if filename.replace('.S', '.o') not in obj_list:
                    file_path = os.path.join(root,filename)
                    file_rel_path = os.path.relpath(file_path, makefile_path)
                    print(file_rel_path)
                    files_asm.append(file_rel_path)
#                    files_asm.append(os.path.join(root,filename))
                    obj_list.append(filename.replace('.S', '.o'))

    out.write('\n.PHONY: all clean mk_build_dir build_lib post_process\n')
    out.write('\ninclude $(ROOT_MK)\n')
    out.write('\nMKDIR= mkdir\n')
    if is_out_lib == 'Y':
        out.write('\nOUTLIB = '+lib_outputf+'\n\n')
    else:
        out.write('\nOUTLIB = \n\n')

    out.write('EXT_CC= ')
    for i in list(ccflags):
        out.write(' '+i)

    for i in list(target_ccflags):
        out.write(' '+i)

    out.write('\n\nEXT_INC=')
    for i in list(local_inc_path):
        out.write(' -I./'+i)

    for i in list(includes):
#        out.write(' "-I'+cwd)
#        out.write('/'+i+'"')
        out.write(' -I./'+i)
    for i in list(inc_path):
        rel_inc_path = os.path.relpath(i, cwd)
        out.write(' -I'+rel_inc_path)

    out.write('\n\nOBJS = ')
    for i in files_c:
        out.write(' '+i.replace('.c', '.o'))
    out.write('\n')

    for i in files_asm:
        i=i.replace('.s', '.o')
        i=i.replace('.S', '.o')
        out.write('\nOBJS += '+i+'\n')
    out.write('\n')

    out.write('\n\nCPPSOURCES = ')
    for i in files_cpp:
        out.write(' "'+i+'"')

    out.write('\n\nLIBS=')
    for i in files_a:
        out.write(' "'+i+'"')

#   out_dir = cwd+'/'+outd
#   out_dir = os.path.join(cwd,outd)
#   if not os.path.exists(out_dir):
#       os.mkdir(out_dir, 0755)
#   out.write('\n\nOUTDIR='+out_dir)

    out.write('\n\nOBJ_LIST =')
    for i in obj_list:
        out.write(' '+i)

#   out.write('\n\nall: debug mk_build_dir $(OBJS) build_lib post_process\n') 
    if is_out_lib == 'Y':
        out.write('\n\nall: $(OBJS) build_lib post_process\n') 
    else:
        out.write('\n\nall: $(OBJS) obj_copy post_process\n') 
    
    out.write('%.o:%.c\n')
#   out.write('\t$(CC) -c $(INCLUDES) $< -o $(OUTDIR)/$(@F)\n\n')
#    out.write('\t$(CC) $(EXT_CC) -c -MMD $(INCLUDES) $(EXT_INC) $< -o $@\n\n')
    out.write('\t$(CC) $(EXT_CC) $(EXT_INC) $< -o $@\n\n')

    out.write('%.o:%.S\n')
#   out.write('\t$(CC) -c $(INCLUDES) $< -o $(OUTDIR)/$(@F)\n\n')
    out.write('\t$(AS) $< -o $@\n\n')


#   out.write('\n\ndebug:\n')
#   out.write('\t@echo $(ROOK_MK)\n')
#   out.write('\t@echo $(CC)\n')

#   out.write('\n\nmk_build_dir:\n')
#   out.write('\t@if [ -d $(OUTDIR) ]; then echo "$(OUTDIR) exist"; else $(MKDIR) -p $(OUTDIR); fi\n')

    out.write('\n\nbuild_lib:\n')
    out.write('\t$(AR) crv $(LIBOUTDIR)/$(OUTLIB) $(OBJS);\n')
    out.write('\tfor f in $(OBJ_LIST); do \\\n') 
    out.write('\t\tif [ -a $(OBJOUTDIR)/$$f ];\\\n')
    out.write('\t\tthen\\\n')
    out.write('\t\t\trm $(OBJOUTDIR)/$$f; \\\n')
    out.write('\t\tfi;\\\n')
    out.write('\tdone\n')
    
    out.write('\n\nobj_copy:\n')
#   out.write('\t-rm -f $(LIBOUTDIR)/$(OUTLIB);\n')
    out.write('\t-cp -f $(OBJS) $(OBJCOPYDIR);\n')

    out.write('\n\npost_process:\n')
    out.write('\t-mv -f *.s $(OBJCOPYDIR);\n')
    out.write('\t-mv -f *.i $(OBJCOPYDIR);\n')

    out.write('\n\nclean:\n')
    out.write('\t-find -name *.o | xargs rm -f\n')
    out.write('\t-find -name *.i | xargs rm -f\n')
    out.write('\t-find -name *.d | xargs rm -f\n')
    out.write('\t-find -name *.s | xargs rm -f\n')
#   out.write('\t-rm -rf $(OUTDIR)\n')
#   out.write('\trm -f *.i\n')
#   out.write('\trm -f *.s\n')

    out.write('\n\n-include $(patsubst %.o,%.d,$(OBJS))\n')
