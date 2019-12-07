def Settings( **kwargs ):
  return {
    'flags': [ 
        '-x', 
        'c++', 
        '-Wall', 
        '-Wextra',
        '-Werror',
        '-std=c++11',
        '-I',
        'usr/include',
        '-isystem',
        '/usr/include/c++',
        '-isystem',
        '/usr/lib/gcc/x86_64-linux-gnu/5.4.0/../../../../include/c++/5.4.0',
        '-isystem',
        '/usr/lib/gcc/x86_64-linux-gnu/5.4.0/../../../../include/x86_64-linux-gnu/c++/5.4.0',
        '-isystem',
        '/usr/local/include',
        '-isystem',
        ' /usr/local/lib/clang/7.0.0/include',
        '-isystem',
        '/usr/include/x86_64-linux-gnu',
        '-isystem',
        '.'
        # project include

        ],
  }
