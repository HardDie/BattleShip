project('BattleShip', 'c')
add_global_arguments('-std=c99', language : 'c')
ncurses = dependency('ncurses')
src = ['main.c', 'game.c', 'global.c', 'draw.c', 'net.c']
executable('BattleShip', src, dependencies : ncurses)
