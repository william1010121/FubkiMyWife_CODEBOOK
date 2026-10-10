" Shell: setxkbmap -option ctrl:nocaps
" ~/.vimrc; select lines, then :Hash (choose one version below).
se nu ai hls et ru ic is sc cul re=1 ts=4 sts=4 sw=4 ls=2 mouse=a
syntax on|hi cursorline cterm=none ctermbg=89|set bg=dark
inoremap {<CR> {<CR>}<Esc>ko<tab>
ca Hash w !cpp -dD -P -fpreprocessed \|
  \ tr -d '[:space:]' \| md5sum \| cut -c-6
ca Hash w !cpp -dD -P \| sed "/^\#.*$/d" \|
  \ tr -d '[:space:]' \| md5sum \| cut -c-6
ca Hash w !g++ -E - \| sed "/^\#.*$/d" \|
  \ tr -d '[:space:]' \| md5sum \| cut -c-6
