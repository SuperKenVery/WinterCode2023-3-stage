project('paopaotang','cpp',
    default_options:[
        'c_std=c17',
        'cpp_std=c++17'
    ]
)

cmake=import('cmake')

sdlpp=dependency('sdl2pp')

executable('main',
    ['main.cpp','Player.cpp','Map/Block.cpp','Map/Map.cpp','Bomb.cpp','BloodBar.cpp'],
    dependencies: [sdlpp],
    install: true,
    install_dir: '/usr/bin',
)

executable('testcompile',
    'test.cpp',
    dependencies: sdlpp
)