pandoc -f markdown -s g191.md -s g191_Annex_A.md -s g191_Annex_B.md -s g191_Bibliography.md  \
    --lua-filter ./md_to_docx/filter.lua \
    --standalone \
    --self-contained \
    -f markdown --reference-doc=./md_to_docx/template.docx -o draft_g191.docx
