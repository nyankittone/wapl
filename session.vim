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
badd +12 term://~//5143:/usr/bin/bash
badd +311 Programming/Wapl/src/app_info.c
badd +23 term://~/Programming/Wapl//5426:/usr/bin/bash
badd +259 Programming/Wapl/src/wapl.h
argglobal
%argdel
edit Programming/Wapl/src/app_info.c
let s:save_splitbelow = &splitbelow
let s:save_splitright = &splitright
set splitbelow splitright
wincmd _ | wincmd |
vsplit
1wincmd h
wincmd w
wincmd _ | wincmd |
split
1wincmd k
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
exe 'vert 1resize ' . ((&columns * 111 + 112) / 225)
exe '2resize ' . ((&lines * 34 + 25) / 51)
exe 'vert 2resize ' . ((&columns * 113 + 112) / 225)
exe '3resize ' . ((&lines * 14 + 25) / 51)
exe 'vert 3resize ' . ((&columns * 113 + 112) / 225)
argglobal
balt term://~//5143:/usr/bin/bash
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
let s:l = 322 - ((40 * winheight(0) + 24) / 49)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 322
normal! 0
lcd ~/Programming/Wapl
wincmd w
argglobal
if bufexists(fnamemodify("~/Programming/Wapl/src/wapl.h", ":p")) | buffer ~/Programming/Wapl/src/wapl.h | else | edit ~/Programming/Wapl/src/wapl.h | endif
if &buftype ==# 'terminal'
  silent file ~/Programming/Wapl/src/wapl.h
endif
balt term://~/Programming/Wapl//5426:/usr/bin/bash
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
let s:l = 260 - ((19 * winheight(0) + 17) / 34)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 260
normal! 0
lcd ~/Programming/Wapl
wincmd w
argglobal
if bufexists(fnamemodify("term://~/Programming/Wapl//5426:/usr/bin/bash", ":p")) | buffer term://~/Programming/Wapl//5426:/usr/bin/bash | else | edit term://~/Programming/Wapl//5426:/usr/bin/bash | endif
if &buftype ==# 'terminal'
  silent file term://~/Programming/Wapl//5426:/usr/bin/bash
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
let s:l = 103 - ((13 * winheight(0) + 7) / 14)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 103
normal! 03|
lcd ~/Programming/Wapl
wincmd w
3wincmd w
exe 'vert 1resize ' . ((&columns * 111 + 112) / 225)
exe '2resize ' . ((&lines * 34 + 25) / 51)
exe 'vert 2resize ' . ((&columns * 113 + 112) / 225)
exe '3resize ' . ((&lines * 14 + 25) / 51)
exe 'vert 3resize ' . ((&columns * 113 + 112) / 225)
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
