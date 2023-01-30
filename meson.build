project('泡泡堂','cpp',
    default_options:[
        'c_std=c17',
        'cpp_std=c++17'
    ]
)

sdlpp=dependency('sdl2pp')

executable('ppt',
    ['main.cpp','Player.cpp','Map/Block.cpp','Map/Map.cpp','Bomb.cpp','BloodBar.cpp','gameScenes.cpp','Bot.cpp'],
    dependencies: [sdlpp],
    install: true,
    install_dir: 'ppt/',
)

install_subdir('res',install_dir:'ppt/')