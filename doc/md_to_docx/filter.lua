
function Para(el)
    if el.content[1].text == "-=Pagebreak=-" then
        return pandoc.RawBlock('openxml', '<w:p><w:r><w:br w:type="page" /></w:r></w:p>')
    else
     return el
    end
 end
