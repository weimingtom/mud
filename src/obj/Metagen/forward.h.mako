<%!
def namespace_begin(n):
    if n.name != '' :
        return namespace_begin(n.parent) + 'namespace ' + n.name + ' {\n'
    else:
        return ''
    
def namespace_end(n):
    if n.name != '' :
        return namespace_end(n.parent) + '}\n'
    else:
        return ''
%>

#pragma once

#include <obj/Config.h>

% for m in module.dependencies :
#include <${ m.subdir }/Generated/Forward.h>
% endfor

% if module.export != '' :
#if defined ${ module.preproc_name }_LIB
#define ${ module.export } MUD_EXPORT
#else
#define ${ module.export } MUD_IMPORT
#endif
% endif

% for key, n in module.root.namespaces.iteritems() :
${ namespace_begin(n) }
    % for e in module.enums :
        % if e.parent == n :
    enum ${ 'class ' if e.scoped else '' }${ e.name } : ${ e.enum_type };
        % endif
    % endfor
    
    % for c in module.templates :
        % if c.parent == n and c.template_used :
    template <class T> ${ 'struct' if c.struct else 'class' } ${ c.template_name };
        % endif
    % endfor
    
    % for c in module.classes :
        % if c.parent == n and not c.nested and not c.is_templated and not c.extern  :
    ${ 'struct' if c.struct else 'class' } ${ c.name };
        % elif c.is_template :
    template <class T> ${ 'struct' if c.struct else 'class' } ${ c.id };
        % endif
    % endfor
${ namespace_end(n) }
% endfor
