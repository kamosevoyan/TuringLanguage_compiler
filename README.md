# In this project realised compiler for language for Turing machine simulator.

### The syntax of the language:

\<program\> = \<symbol list\> \<main-statement\>

\<main-statement\> = \<statement\>

\<statement\> =  \<block-statement\> | \<if-statement\> | \<if_else-statement\> | \<ifnot-statement\> | \<if-statement\> |  \<while-statement\> | \<do_while-statement\>|
            \<repeat_until-statement\> |  \<write-statement\> |  \<left-statement\> |  \<right-statement\> |  \<exit-statement\> |  \<error-statement\>
            \<continue-statement\> | \<break-statement\>
              
\<block-statement\> = '{' \<statement\>+ '}'
  
\<if-statement\> = 'if' \<symbol list\> \<statement\>

\<if_else-statement\> = 'if' \<symbol list\> \<statement\> 'else' \<statement\>
              
\<ifnot-statement\> = 'ifnot' \<symbol list\>  \<statement\>
              
\<while-statement\> = 'while' \<symbol list\> \<statement\>

\<do_whlie-statement\> = 'do' \<statement\> 'while' \<symbol list\> ';'

\<repeat_until-statement\> = 'repeat' \<statement\> 'until' \<symbol list\> ';'
              
\<write-statement\> = 'write' \<symbol\> ';'
              
\<left-statement\> = 'left' ';'
              
\<right-statement\> = 'right' ';'
              
\<exit-statement\> = 'exit' ';'
              
\<error-statement\> = 'error' ';'
              
\<continue-statement\> = 'cointinue' ';'

\<break-statement\> = 'break' ';'
              
\<symbol list\> = '(' \<symbol\> (\<comma\> \<symbol\>)* ')'

\<symbol\> =  ASCII printable character

\<comma\> = ','  
                            
              

              
              
              
              
              
              
