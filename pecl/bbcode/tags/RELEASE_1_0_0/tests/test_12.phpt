--TEST--
BBCODE Param Sub Parsing
--FILE--
<?php
if (!extension_loaded('bbcode')){
        if (!dl('bbcode.so')){
                die ('bbcode not loadable');
        }
}
$arrayBBCode=array(
    'i'=>        array('type'=>BBCODE_TYPE_ARG, 'open_tag'=>'<i {PARAM}>',
                    'close_tag'=>'</i>',
                    'flags'=>BBCODE_FLAGS_ARG_PARSING
                    ),
    'b'=>        array('type'=>BBCODE_TYPE_NOARG, 'open_tag'=>'<b>',
                    'close_tag'=>'</b>'),
);
$text=<<<EOF
[i="[b]Parsing_test[/b]"]Italic Text[/i]
[i=&quot;[b]Parsing_test_html[/b]&quot;]Italic Text[/i]
EOF;
$BBHandler=bbcode_create($arrayBBCode);
echo bbcode_parse($BBHandler,$text);
?>
--EXPECT--
<i <b>Parsing_test</b>>Italic Text</i>
<i <b>Parsing_test_html</b>>Italic Text</i>