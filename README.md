# In this project compiler for language for Turing machine simulator are realised.

### The syntax of the language:

\<program\> = \<symbol list\> \<main-statement\>

\<main-statement\> = \<statement\>

\<statement\> =  \<block-statement\> | \<if-statement\> | \<if_else-statement\> | \<ifnot-statement\> | \<while-statement\> | \<do_while-statement\>| \<repeat_until-statement\> | \<write-statement\> | \<left-statement\> | \<right-statement\> | \<exit-statement\> | \<error-statement\> | \<continue-statement\> | \<break-statement\>
              
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
              
\<symbol list\> = '(' \<symbol\> ( \<comma\> \<symbol\> )* ')'

\<symbol\> =  ASCII printable character

\<comma\> = ','

## Description of statements.

* if statement <br/>
The statements of **if statements** are executed only if the symbol in the current cell of the tape is available in symbol list.

* if-else statement <br/>
The **if branch statements** of **if-else statements** are executed when the symbol in the current cell of the tape is available in symbol list, otherwhies the **else branch statements** are exequted.

* ifnot statement <br/>
The statements of **ifnot statements** are executed only if the symbol in the current cell of the tape is not available in symbol list.

* ifnot statement <br/>
The statements of **ifnot statements** are executed only if the symbol in the current cell of the tape is not available in symbol list.

* while statement <br/>
The statements of **while statements** are executed while the symbol in the current cell of the tape is available in symbol list.

* do_while statement <br/>
The statements of **do_while statements** are executed while the symbol in the current cell of the tape is available in symbol list.
In contrast of **while statement**, the statements are eqequted at least once.

* repeat_until statement <br/>
The statements of **repeat_until statements** are executed until the symbol in the current cell of the tape is available in symbol list.

* write statement <br/>
Writes the symbol in the currect cell of the tape.

* left statement <br/>
Turns the head of the machine left.

* right statement <br/>
Turns the head of the machine left.

* exit statement <br/>
Halts the machine.
              
* error statement <br/>
Gives error command to the machine to halt unsuccessfully.
                            
* continue/break statements <br/>
These statements are used as flow controls in **while**, **do_while** and **repeat_until** loops.
<br/> If these statements aren't contained in any of loops given above, the parser trows an exception.

## The compiler pareses program using tree based algorithm.
                            
              

              
              
              
              
              
              
