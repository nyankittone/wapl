let SessionLoad = 1
let s:so_save = &g:so | let s:siso_save = &g:siso | setg so=0 siso=0 | setl so=-1 siso=-1
let v:this_session=expand("<sfile>:p")
silent only
silent tabonly
cd ~
if expand('%') == '' && !&modified && line('$') <= 1 && getline(1) == ''
  let s:wipebuf = bufnr('%')
endif
let s:shortmess_save = &shortmess
if &shortmess =~ 'A'
  set shortmess=aoOA
else
  set shortmess=aoO
endif
badd +385 term://~//7814:/usr/bin/bash
badd +178 Programming/Wapl/src/wapl.h
badd +27 term://~//213035:/usr/bin/bash
badd +2 Programming/Wapl_Test/src/main.rs
badd +864 term://~/Programming/Wapl_Test//215036:/usr/bin/bash
badd +11 Programming/Wapl_Test/Cargo.toml
badd +2 Programming/Wapl_Test/build.rs
badd +7 Programming/Wapl/src/main.c
badd +2 Programming/Wapl/mk_posix.sh
badd +5 Programming/Wapl/compile_flags.txt
badd +5 Programming/Wapl/examples/clap/.gitignore
argglobal
%argdel
edit Programming/Wapl/examples/clap/.gitignore
let s:save_splitbelow = &splitbelow
let s:save_splitright = &splitright
set splitbelow splitright
wincmd _ | wincmd |
vsplit
1wincmd h
wincmd w
let &splitbelow = s:save_splitbelow
let &splitright = s:save_splitright
wincmd t
let s:save_winminheight = &winminheight
let s:save_winminwidth = &winminwidth
set winminheight=0
set winheight=1
set winminwidth=0
set winwidth=1
exe 'vert 1resize ' . ((&columns * 112 + 112) / 225)
exe 'vert 2resize ' . ((&columns * 112 + 112) / 225)
tcd ~/Programming/Wapl
argglobal
balt ~/Programming/Wapl/src/wapl.h
setlocal foldmethod=manual
setlocal foldexpr=0
setlocal foldmarker={{{,}}}
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldenable
silent! normal! zE
let &fdl = &fdl
let s:l = 5 - ((4 * winheight(0) + 24) / 49)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 5
normal! 0
lcd ~/Programming/Wapl
wincmd w
argglobal
if bufexists(fnamemodify("term://~//7814:/usr/bin/bash", ":p")) | buffer term://~//7814:/usr/bin/bash | else | edit term://~//7814:/usr/bin/bash | endif
if &buftype ==# 'terminal'
  silent file term://~//7814:/usr/bin/bash
endif
balt ~/Programming/Wapl/src/wapl.h
setlocal foldmethod=manual
setlocal foldexpr=0
setlocal foldmarker={{{,}}}
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldenable
let s:l = 385 - ((48 * winheight(0) + 24) / 49)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 385
normal! 03|
lcd ~/Programming/Wapl
wincmd w
2wincmd w
exe 'vert 1resize ' . ((&columns * 112 + 112) / 225)
exe 'vert 2resize ' . ((&columns * 112 + 112) / 225)
tabnext 1
if exists('s:wipebuf') && len(win_findbuf(s:wipebuf)) == 0 && getbufvar(s:wipebuf, '&buftype') isnot# 'terminal'
  silent exe 'bwipe ' . s:wipebuf
endif
unlet! s:wipebuf
set winheight=1 winwidth=20
let &shortmess = s:shortmess_save
let &winminheight = s:save_winminheight
let &winminwidth = s:save_winminwidth
let s:sx = expand("<sfile>:p:r")."x.vim"
if filereadable(s:sx)
  exe "source " . fnameescape(s:sx)
endif
let &g:so = s:so_save | let &g:siso = s:siso_save
doautoall SessionLoadPost
unlet SessionLoad
" vim: set ft=vim :
