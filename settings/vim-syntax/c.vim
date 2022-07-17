" set the colors for spell checking
hi SpellBad ctermbg=grey ctermfg=red
hi SpellCap ctermbg=yellow ctermfg=blue
hi spellRare ctermbg=green ctermfg=black
hi SpellLocal ctermbg=blue ctermfg=red
hi SystemDefine ctermfg=blue
hi SystemVar ctermfg=red
hi SpecialFunc ctermfg=magenta
hi SpecialKeyword ctermfg=yellow
hi SpecialGreen ctermfg=2

" Color Defines
hi Black ctermfg=0
hi Red ctermfg=1
hi Green ctermfg=2
hi Orange ctermfg=3
hi Blue ctermfg=4
hi Purple ctermfg=5
hi LightBlue ctermfg=6

" Windows
syn keyword SpecialFunc __declspec
syn keyword SystemDefine __stdcall
syn keyword SystemDefine dllimport
syn keyword SystemDefine dllexport
syn keyword SystemDefine WINAPI
syn keyword SpecialKeyword gcnew
syn keyword Type DllMain
syn keyword Type WinMain

syn keyword SpecialFunc __PRETTY_FUNCTION__

" C/C++
syn keyword SpecialFunc nullptr
syn keyword SpecialFunc calloc
syn keyword SpecialFunc malloc
syn keyword SpecialFunc realloc
syn keyword SpecialFunc free
syn keyword SpecialFunc DLLEXPORT
syn keyword SpecialFunc mem_free
syn keyword SpecialFunc OPENSSL_malloc
syn keyword SpecialFunc OPENSSL_free
syn keyword SpecialFunc delete
syn keyword SpecialFunc new

" Legacy firmware typedefs
syn keyword Type int8
syn keyword Type uint8
syn keyword Type int32
syn keyword Type uint32

" Public namespaces
syn keyword SpecialGreen std

" General
syn keyword SystemVar errno
syn keyword Title EWOULDBLOCK
syn keyword Title EISCONN
syn keyword Title ECONNRESET
syn keyword cTodo NOTE
syn keyword cTodo Note
syn keyword cTodo THREEDO

" Glory Health & Wealth
syn keyword Type ghw
syn keyword Type GHW
syn keyword Type iGHW
syn keyword Type llGHW
syn keyword Type lGHW
syn keyword Type uGHW
syn keyword Type usGHW
syn keyword Type sGHW
syn keyword Type luGHW
syn keyword Type dbiGHW
syn keyword type fGHW
syn keyword Type strGHW
syn keyword Type szGHW
syn keyword Type pszGHW
syn keyword Type cGHW
syn keyword Type ucGHW
syn keyword Type acGHW
syn keyword Type aucGHW
syn keyword Type pacGHW
syn keyword Type paucGHW
syn keyword Type GHWChronic

" Highlight strings and numbers in comments
let c_comment_strings = 1
syn region cCode start=">>>>" end="$" contains=cFormat,cCppString,cLabel,cConditional,cRepeat,cCharacter,cSpecialCharacter,cNumber,cOctal,cOctalZero,cFloat,cOctalError,cErrInParen,cCommentError,cCommentStartError,cSpaceError,cSpecialError,cCurlyError,cOperator,cStructure,cStorageClass,cInclude,cPreProc,cDefine,cIncluded,cError,cStatement,cCppInWrapper,cCppOutWrapper,cPreConditMatch,cPreCondit,cType,cConstant,cCommentString,cComment2String,cCommentSkip,cString,cComment,cSpecial,cTodo,cBadContinuation,cCppOutSkip,cCppInElse2,cCppOutIf2,cCppOut2,cCppOut,SystemVar,cTodo,Type,cOperator,SystemDefine,SystemVar,SpecialFunc,SpecialKeyword
syn region cComment matchgroup=cCommentStart start="/\*" end="\*/" contains=cCode,@cCommentGroup,cCommentStartError,cCommentString,cCharacter,cNumbersCom,cSpaceError,@Spell

