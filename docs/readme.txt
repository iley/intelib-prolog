1. Introduction

  This directory contains documentation files for the InteLib and
accompanying software.  I'm working hard to make the documentation as
complete and relevant as possible, but unfortunately I'm short of time.
Well, anyway remember this is not yet a release.  I promise not to do an
official release of the software until I bring the documentation to an
acceptable level.  I would also warmly welcome any contributions to the
documentation.  The actual version is always available from
http://www.intelib.org and ftp://ftp.croco.net/intelib.

  Please note the source files of the library are always available to you,
so be sure to take a look at them if you're in doubt or/and confused with
any problem.  The headers as well as the sources include many comments
which in many cases may help you. 

  At the site http://www.intelib.org there's a Wiki framework devoted
to InteLib.  You might want to use that kind of forum to discuss InteLib and
related questions with the author as well as with other interested people.

  Mailing list (list@intelib.org) is also available.  Please send a message
with the word 'subscribe' within it's body to list-request@intelib.org if
you wish to subscribe.

  As a very last resort you can contact me via email avst --at-- cs.msu.ru.
I'll try hard to answer all the questions you have.  Any feedback, bug
reports etc are always welcome. 


                                                  Certainly yours, 
                                                  Andrey Stolyarov  

2. Licensing information


  InteLib is free software. 

  The library part (the sexpress/, tools/, genlisp/, lisp/, scheme/ and
refal/ directories) is available under GNU Lesser General Public License,
v.2.1. 

  All the InteLib files outside of these directories (including, but not
limited to, the interp/, ill/ and ils/ directories) are available under 
GNU General Public License, v.2.

  See the files gnu_gpl2.txt and gnu_lgpl21.txt for the official
texts of the respective licenses.

  Please note that Lesser General Public License does *NOT* allow using the
library in generic proprietary software; you need to follow some
non-trivial steps in order to comply this license, including, but not
limited to, making the source code of the library available for your user
and (!) providing the user the ability to relink the whole project in case
she decides to modify the library.

  Please find some time to take a look at http://www.intelib.org/license.html


3. Index of the documentation files

 - readme.txt      General concept overview and the documentation index
                   (this text). 
 - gnu_lgpl21.txt  GNU Lesser General Public License v.2.1 as published 
                   by FSF;
 - gnu_gpl2.txt    GNU General Public License v.2 as published by FSF;
 - lispkern.txt    InteLib library reference manual. Explanation of
                   main classes and their interfaces, etc. 
 - ill_lang.txt    InteLib Lisp language definition and reference. 
 - ill.txt         documentation on the ILL translator which translates
                   files written in traditional Lisp syntax into a C++ 
                   modules.  
 - build.txt       How to build the library and bootstrap the translator
 - win_mingw.txt   Basic instructions on building under Windows
 - lispfuns/*      Information on IntelibLisp-specific functions; see
                   lispfuns/readme.txt.

4. General concept overview

  The name 'InteLib' first appeared to stand for 'Intelligence Library'
to reflect the fact the primary general goal of the project was to allow
a programmer to use so-called 'artificial intelligence' language group
in a daily practice. 
  The package is intended to make practical use of Lisp language and 
the programming paradigms developed in its environment. It's no surprise 
that some very interesting programming paradigms appeared in artificial 
intelligence languages such as Lisp, Prolog, Refal, Planner etc. are not
in use nowadays because they are in most cases not suitable to implement a
whole project. A chief programmer usually choose an object-oriented
imperative language such as C++, Ada, Java etc as the primary (and the
only) language of the project. 
  It's often a good idea to implement a subtask, a module or two, in a
language different from the primary one, but it's too hard to merge
modules together.  All these calling conventions, data sharing conventions,
bla bla bla -- these things provide a programmer with such a perfect
headache that in most cases the possibility of using different languages
is not even considered.
  The InteLib project provides a completely new approach to multilingual
and multiparadigm programming.  We call it 'immediate integration'.  For
now, it is implemented for C++ as the primary and Scheme & Lisp as a
secondary language (I plan not to stop at this point, so look forward for
new releases).

  As you know, there's a basic notion of 'S-expression' in Lisp. It's very
fundamental for Lisp. Both program and data in Lisp are built of
S-expressions so one may say S-expression is "anything and everything" in
Lisp.
  Having an object-oriented style of thinking, we can immediately come up
with an idea to represent S-expression with a class, or, more precisely,
with a polymorphic hierarchy of classes. Then, having rich syntactic
capabilities of C++, we can overload well-known operations in a way which
will provide us with convenient instruments for handling S-expressions
(build Lisp lists etc). In the InteLib library, the basic class of the
hierarchy is called SExpression. It is an abstract class; it has many derived
classes for different types of Lisp S-expression. See lispkern.txt and
certailny the library sources for details.
  Another class, named SReference, has some knowledge aboud several
basic classes of S-expressions. It acts as a smart pointer to the
S-expressions providing operations to build and analyse Lisp lists. It
also provides a very simple technique of garbage collecting -- namely
the reference counting.  If you use one of the available C++ garbage
collectors, you can turn this off. 
  Let's now recall that comma (,) is a operator in C/C++ and we can
overload it for our classes. It looks natural to use commas to separate
items in a list (yes, in Lisp a whitespace is used for this purpose, but
whitespace is not an operator in C++). Now one may expect something like 

   (SReference(25), SReference(36), SReference(49)) 

as a representation of Lisp list (25 36 49). Well, in fact it is
sufficient to have one of operands to be of a special class in order to
have the compiler applying the overloaded operator, so it could look like
this:

             (SReference(25), 36, 49)  

However, what do we do to represent something like ((25 36) 49)? Ok, we've
got a constuctor of SReference which takes two SReferences and builds a
dotpair, so we could use something like this:

     (SReference((SReference(25), 36), NULL), 49)

Totally unreadable, isn't it? And, by the way, we have to differ semantics
of the comma operator depending whether the left-handed operand is an atom
or list. This also doesn't make things simpler. 
  The problem is solved with a special class named SListConstructor. It
has no data members, only several overloaded operators. The main goal of
the class is to provide an additional 'unary' operation which turns
anything (not only SExpressions/SReferences, and that's why we need a special
class to place at the left-side operand...) - turns anything into a Lisp
list of exactly one element. Usually we've got exactly one instance of
SListConstructor named `L'. The overloaded operator is vertical bar (|)
normally used for bitwise OR operation. So we can write, for example, the
following: 

   (L| 25, 36, 49)                       // (25 36 49)
   (L| "I am the walrus", 1965)          // ("I am the walrus" 1965)
   (L| 1, 2, (L| 3, 4), 5, 6)            // (1 2 (3 4) 5 6)
   (L| (L| 1, 2), 3, 4)                  // ((1 2) 3 4)
   (L| (L| (L| (L))))                    // (((())))

The last example is possible because LListConstructor being cast to an
SReference gives an 'empty list'.
  Besides that, we need an operation to represent dotted pairs and dotted
lists. Unfortunately, there are no operations in C++ with a lower priority
than comma so representations of dotted lists will look a little bit
unnatural. Anyway, we definitely need an operation which has lower
priority than |. The choice is not rich: ||, && and various assignment
operators. I've choosen || so we now can write a code like this:

   (L| 1 || 2)                           // (1 . 2)
   ((L| 1, 2, 3, 4) || 5)                // (1 2 3 4 . 5)



5. Lisp computational model


  SExpressions and SReferences know nothing about Lisp evaluations. In early 
versions of InteLib, this wasn't true, and Evaluate() was one of the virtual
methods of SExpression (which was called LTerm these prehistoric times ;-)
Later I've found that S-expressions are sometimes good without being 
evaluated, for example, if we need heterogenous data structures. They are
also good in implementing other languages' paradigms, such as that of Prolog,
Datalog, Refal and many others. In fact, S-expressions are popular outside
of Lisp, but not all people realize that.
  Anyways, we've got sometimes to evaluate S-expressions. For this purpose, 
the classes SReference and SListConstructor should be replaced with 
their descenders LReference and LListConstructor (or, if we like the Scheme
computation model, with SchReference and SchListConstructor, respectively),
which are intended to handle the Lisp (or Scheme) computations. 
  These class even add another overloaded operator, unary ~ (bitwise not) 
intended to replace Lisp's apostrophe ('A is an abbreviated form of 
(QUOTE A)).  This makes it possible to write:

   (L| APPEND, ~(L| 1, 2), ~(L| 3, 4))   // (APPEND '(1 2) '(3 4))
   (L| SETQ, A, ~B)                      // (SETQ A 'B)

In order to make the S-expressions live, we need a mechanism of their
evaluation. LReference has the method Evaluate() for this purpose. So

   (L| APPEND, ~(L| 1, 2), ~(L| 3, 4)).Evaluate()

is a valid C++ expression which returns an LReference representing Lisp
list (1 2 3 4). We could write, for example:

   LReference result = (L| APPEND, ~(L| 1, 2), ~(L| 3, 4)).Evaluate();

in our C++ program. I call such constructs as "Lisp-like C++ code". 


6. InteLib Lisp - what is it?

  It is important to understand that C++ remains being C++ regardless of
what libraries are used, so the shown code is really C++ code, not
depending how much does it look like Lisp.  So in fact the InteLib library
helps to integrate programming paradigms, not languages.
  You can notice tough that I always talk about 'representation of Lisp
constructs' with C++ code.  We know how many different dialects of Lisp
exist.  Well, what particular Lisp is represented with our C++ 'Lisp-like
code'?
  The answer is not so easy.  It certainly could be possible to implement
one more Common Lisp or Scheme, but these standards look too much like
monsters.  In multilingual environment, we only need those paradigms
existing in Lisp that doesn't exist in our primary language so our Lisp
dialect should be dramatically more simple than Common Lisp (well, if you
try to show all the world you can implement anything and everything with
Lisp, you'll definitely end up with a monster, heh... but with InteLib
Lisp we are not going to implement everything - we're going to use it
only for subtasks where Lisp is more suitable than C++).  So I decided not
to try cloning any existing Lisp dialect. Instead, I've just implemented
those Lisp's features I need in my work. At the beginning of the project I
thougt I'll make a strict subset of Common Lisp. At later stages, however,
I've found useful some features of other Lisps (including Scheme), so now
it is a new dialect.  I call it InteLib Lisp.
  Please note InteLib Lisp is a dialect of Lisp, not a set of C++
constructs.  A C++ code is a C++ code, and therefore a construct such as     
(L| APPEND, ~(L| 1, 2), ~(L| 3, 4)) doesn't belong to any Lisp; it belongs
to C++. It does, however, _represent_ the expression (APPEND '(1 2) '(3 4))
which belongs to a Lisp -- to InteLib Lisp.
  The dialect could exist in my mind only if there was no translator of
it. At one of the very early stages of my project, however, I noticed that
it is inconvenient to write large code fragments with the InteLib syntax.
At the same time, a translator from Lisp in its traditional syntax into
C++ code which uses InteLib capabilities is quite simple. We only need to
replace every opening bracket with `(L|', replace whitespace with commas
wherever appropriate, add the necessary 'Evaluate()' calls and make some
minor transformations. 
  The first version of the translator was named cl2cpp. It was first
implemented for the previous version of the library (demo, very slow and
stupid, but working). Now it is obsolete. 
  The current version is called ILL (the name stands for (I)nte(L)ib
(L)isp). See ill.txt for details.  Having the translator, we can say that
InteLib Lisp is a real existing dialect of Lisp. 
  There's an interactive interpreter named NILL (abbreviation from
iNteractive InteLib Lisp).  It uses the same classes so it is guaranteed it
supports the same dialect.  The interpreter even supports Gnu Readline, and
is perfectly embeddable, but it is nevertheless intended for debugging
purposes and definitely NOT for using as a stand-alone Lisp interpreter
(pick any Common Lisp implementation if you need one).
