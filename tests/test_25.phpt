--TEST--
BBCODE Param Quote escaping
--FILE--
<?php
if (!extension_loaded('bbcode')){
        if (!dl('bbcode.so')){
                die ('bbcode not loadable');
        }
}
$arrayBBCode=array(
    'center'=>    array('type'=>BBCODE_TYPE_NOARG,                        'open_tag'=>'<div style="text-align:center;">',                'close_tag'=>'</div>'),
    'strike'=>    array('type'=>BBCODE_TYPE_NOARG,                        'open_tag'=>'<span style="text-decoration:line-through;">',    'close_tag'=>'</span>'),
    'color'=>    array('type'=>BBCODE_TYPE_ARG,                            'open_tag'=>'<span style="color:{PARAM}">',                    'close_tag'=>'</span>'),
);
$text=<<<EOF
[color=navy]Navy Text[/color]
[color='red\'']Red Text[/color]
[color="green\""]Green Text[/color]
[color=&quot;blue\&quot;Test&quot;]Blue Text[/color]
EOF;
$BBHandler=bbcode_create($arrayBBCode);
bbcode_set_flags($BBHandler,BBCODE_ARG_QUOTE_ESCAPING,BBCODE_SET_FLAGS_ADD);
echo bbcode_parse($BBHandler,$text);
?>
--EXPECT--
<span style="color:navy">Navy Text</span>
<span style="color:red'">Red Text</span>
<span style="color:green"">Green Text</span>
<span style="color:blue&quot;Test">Blue Text</span>