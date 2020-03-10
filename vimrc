" Colors/Fonts etc.
colorscheme evening 

" Syntax highlighting etc.
filetype on

"call pathogen#infect()

"set nowrap
"set number " line number
set autoindent
set smartindent
set cindent
set noexpandtab
set tabstop=4       " Force tabs to be displayed/expanded to 4 spaces (instead of default 8)
set shiftwidth=4    " When auto-indenting, indent by this much. (Use spaces/tabs per "expandtab".)
set nocompatible
set modelines=0
"set foldenable
"set foldlevelstart=10
"set foldnestmax=10
"set foldmethod=indent
set scrolloff=2
set matchpairs+=<:>

set showmode
set showcmd
set hidden
set wildmenu
set wildmode=list:longest
set visualbell
"set cursorline
set ttyfast
set ruler
set backspace=indent,eol,start
set laststatus=2
set confirm
" set relativenumber
" set undofile

" Searching
set ignorecase
set smartcase
set gdefault
set incsearch
set showmatch
set hlsearch
syntax enable

" NERD Tree

" Tagbar
nmap <F2> :TagbarToggle<CR>

" Tags	
set tags=/home/d7/Desktop/LinuxPrac/tags;/ 

" File Header
map <F2>c i/**************************************** MAD Engineers ****************************************<CR><BS><BS><CR>MAD Engineers<CR>Copyright (c) 2014<CR><CR>Description	: <CR><CR>Revision History **************************************************************************<CR>** Date ** ** Coder ** ***** Remarks ******************************************************<CR>* DDMMYY * ** madD7 ** * Originator                                                       *<CR><CR>*/<CR><CR><CR><CR>/* Include Files **************************************************************************<CR><BS><BS>* @{<CR>*/<CR><CR>/*<CR><BS><BS>* @}<CR>*/<CR><CR><CR>/* Variables ******************************************************************************<CR><BS><BS>* @{<CR>*/<CR><CR><CR>/*<CR><BS><BS>* @}<CR>*/<CR><CR><CR><CR>/* Functions ******************************************************************************<CR><BS><BS>* @{<CR>*/<CR><CR><CR>/*<CR><BS><BS>* @}<CR>*/<CR><CR><CR><CR><CR><CR>/* ~~~~~ END OF FILE ~~~~~ */<CR><Esc>11kA
map <F2>h i/**************************************** MAD Engineers ****************************************<CR><BS><BS><CR>MAD Engineers<CR>Copyright (c) 2014<CR><CR>Description	: <CR><CR>Revision History **************************************************************************<CR>** Date ** ** Coder ** ***** Remarks ******************************************************<CR>* DDMMYY * ** madD7 ** * Originator                                                       *<CR><CR>*/<CR><CR><CR><CR>#ifndef		__filename_H_     //Replace filename<CR>#define		__filename_H_     //Replace filename<CR><CR><CR><CR>/* Include Files **************************************************************************<CR><BS><BS>* @{<CR>*/<CR><CR>/*<CR><BS><BS>* @}<CR>*/<CR><CR><CR>/* Data Types & Variables *****************************************************************<CR><BS><BS>* @{<CR>*/<CR><CR><CR>/*<CR><BS><BS>* @}<CR>*/<CR><CR><CR><CR>/* Functions ******************************************************************************<CR><BS><BS>* @{<CR>*/<CR><CR><CR>/*<CR><BS><BS>* @}<CR>*/<CR><CR><CR>#endif<CR><CR><CR>/* ~~~~~ END OF FILE ~~~~~ */<CR><Esc>39kA
map <F2>f i/***************************************************************************************<CR>Description	: <CR>Input		: <CR>Output		: None<CR>Returns		: <CR>Notes		: None<CR>*/<ESC>6kA
map <F2>d i/***************************************************************************************<CR><BS><BS>Data Structure Name	: <CR>Description	: <CR>Notes		: None<CR>*/<ESC>3kA

map <F2>p i#!/opt/python3.3/bin/python3.3<CR>'''<CR>*************************************** MAD Engineers ****************************************<CR><BS><BS><CR>MAD Engineers<CR>Copyright (c) 2014<CR><CR>Description   : <CR><CR>Revision History **************************************************************************<CR>** Date ** ** Coder ** ***** Remarks ******************************************************<CR>* DDMMYY * ** madD7 ** * Originator                                                       *<CR><CR>'''<CR><CR><CR><CR># End of File

map <F3>b i#!/bin/bash<CR><CR>#*************************************** MAD Engineers ****************************************<CR><BS><BS><CR>#MAD Engineers<CR>#Copyright (c) 2014<CR><CR>#Description   : <CR><CR>#Revision History **************************************************************************<CR>#** Date ** ** Coder ** ***** Remarks ******************************************************<CR>#* DDMMYY * ** madD7 ** * Originator                                                       *<CR>#<CR><CR><CR><CR><CR># End of File


" Ctrl+S to save file
" nnoremap <c-s> :w<CR>
" inoremap <c-s> <esc>:w<CR>a

inoremap <c-z> <c-o>:u<CR>
