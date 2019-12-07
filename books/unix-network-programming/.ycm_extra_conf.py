def Settings( **kwargs ):
    return {
            'flags': [ 
                '-x', 
                'c++', 
                '-Wall', 
                '-Wextra', 
                '-Werror',
                '-isystem',
                'lib',
                '-isystem',
                'key',
                '-isystem',
                'include',
                 '-isystem',
                'sctp'
            ],
            }
