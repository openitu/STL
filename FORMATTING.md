The code is formatted using [GNU indent](https://www.gnu.org/software/indent/).

```
indent \
--braces-after-if-line \
--braces-after-func-def-line \
--braces-on-if-line \
--braces-on-func-def-line \
--braces-on-struct-decl-line \
--break-after-boolean-operator \ 
--break-before-boolean-operator \
--cuddle-else \
--dont-break-function-decl-args \
--dont-break-function-decl-args-end \
--dont-break-procedure-type \
--dont-star-comments \
--format-all-comments \
--leave-optional-blank-lines \
--space-after-cast \
--no-tabs \
-l999 \
*.{c,h,cpp,hpp}
```
