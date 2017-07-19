#coding:utf-8
#上一行的“coding:utf-8”不是一个注释内容，而是一个声明，允许以utf-8进行注释。
#它必须放在python文件的第一行，然后下文就可以用中文进行注释了。否则python
#不支持中文注释，读取含有中文的python文件是出错。

#查找编译器默认包含路径的方法：g++ -E -x c++ - -v < /dev/null
#或者 clang++ -E -x c++ - -v < /dev/null
#然后输出的内容从 “#include <...> search starts here:” 到
#“End of search list.”的内容就是编译器的默认包含路径。

import os 
import ycm_core

curdir = os.getcwd()                                        #get current directory 
split = curdir.split('/')                                   #splet by '/'
QT_Ui_Dir = '../build-' + split[-1] + '-QT5_6_0_MinGW64_5_3_0-Debug'   #QT Ui folder

flags = [  
    '-Wno-deprecated-declarations', 
    '-Wno-disabled-macro-expansion', 
    '-Wno-c++98-compat', 
    '-Wno-c++98-compat-pedantic', 
    '-Wno-global-constructors', 
    '-Wno-exit-time-destructors', 
    '-Wno-missing-prototypes', 
    '-Wno-padded',

    '-Wunused',
    '-Wuninitialized',
    '-Wall',
    '-Wextra',
    '-Wpointer-arith',
    '-Wshadow',
    '-Wfloat-equal', 

    '-x', 'c++',
    '-std=c89',

    '-isystem', r'E:/ProgramFiles/mingw-w64/x86_64-5.3.0-win32-seh-rt_v4-rev0/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/5.3.0/include',
    '-isystem', r'E:/ProgramFiles/mingw-w64/x86_64-5.3.0-win32-seh-rt_v4-rev0/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/5.3.0/include-fixed',
    '-isystem', r'E:/ProgramFiles/mingw-w64/x86_64-5.3.0-win32-seh-rt_v4-rev0/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/5.3.0/../../../../x86_64-w64-mingw32/include',
    '-isystem', r'E:/ProgramFiles/mingw-w64/x86_64-5.3.0-win32-seh-rt_v4-rev0/mingw64/lib/gcc/../../x86_64-w64-mingw32/include/c++',
    '-isystem', r'E:/ProgramFiles/mingw-w64/x86_64-5.3.0-win32-seh-rt_v4-rev0/mingw64/lib/gcc/../../x86_64-w64-mingw32/include/c++/x86_64-w64-mingw32',
    '-isystem', r'E:/ProgramFiles/mingw-w64/x86_64-5.3.0-win32-seh-rt_v4-rev0/mingw64/lib/gcc/../../x86_64-w64-mingw32/include/c++/backward',

    '-I', '.',
    '-I', '../include',
] 
compilation_database_folder = '' 
if compilation_database_folder: 
  database = ycm_core.CompilationDatabase( compilation_database_folder ) 
else: 
  database = None 
SOURCE_EXTENSIONS = [ '.cpp', '.cxx', '.cc', '.c', '.m', '.mm' ] 
def DirectoryOfThisScript(): 
  return os.path.dirname( os.path.abspath( __file__ ) ) 
def MakeRelativePathsInFlagsAbsolute( flags, working_directory ): 
  if not working_directory: 
    return list( flags ) 
  new_flags = [] 
  make_next_absolute = False 
  path_flags = [ '-isystem', '-I', '-iquote', '--sysroot=' ] 
  for flag in flags: 
    new_flag = flag 
    if make_next_absolute: 
      make_next_absolute = False 
      if not flag.startswith( '/' ): 
        new_flag = os.path.join( working_directory, flag ) 
    for path_flag in path_flags: 
      if flag == path_flag: 
        make_next_absolute = True 
        break 
      if flag.startswith( path_flag ): 
        path = flag[ len( path_flag ): ] 
        new_flag = path_flag + os.path.join( working_directory, path ) 
        break 
    if new_flag: 
      new_flags.append( new_flag ) 
  return new_flags 
def IsHeaderFile( filename ): 
  extension = os.path.splitext( filename )[ 1 ] 
  return extension in [ '.h', '.hxx', '.hpp', '.hh' ] 
def GetCompilationInfoForFile( filename ): 
  if IsHeaderFile( filename ): 
    basename = os.path.splitext( filename )[ 0 ] 
    for extension in SOURCE_EXTENSIONS: 
      replacement_file = basename + extension 
      if os.path.exists( replacement_file ): 
        compilation_info = database.GetCompilationInfoForFile(          replacement_file )
        if compilation_info.compiler_flags_: 
          return compilation_info 
    return None 
  return database.GetCompilationInfoForFile( filename ) 
def FlagsForFile( filename, **kwargs ): 
  if database: 
    compilation_info = GetCompilationInfoForFile( filename ) 
    if not compilation_info: 
      return None 
    final_flags = MakeRelativePathsInFlagsAbsolute( 
      compilation_info.compiler_flags_, 
      compilation_info.compiler_working_dir_ ) 
  else: 
    relative_to = DirectoryOfThisScript() 
    final_flags = MakeRelativePathsInFlagsAbsolute( flags, relative_to ) 
  return { 
    'flags': final_flags, 
    'do_cache': True 
  }
