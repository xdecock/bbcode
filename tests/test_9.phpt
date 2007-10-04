--TEST--
BBCODE Callback
--FILE--
<?php
if (!extension_loaded('bbcode')){
        if (!dl('bbcode.so')){
                die ('bbcode not loadable');
        }
}
Class Foo{
	static function bar($content,$param){
		return "Foo.bar ".$content;
	}
	function __construct(){
	}
	function foo_cb($content, $param) {
		return "Foo.foo ".$content;
	}
}
function callback($content, $param){
	return "Callback ".$content;
}
$foo=new Foo;
$arrayBBCode=array(
    'i'=>        array('type'=>BBCODE_TYPE_NOARG, 'open_tag'=>'<i>',
                    'close_tag'=>'</i>', 'content_handling'=>array('Foo','bar')),
    'b'=>        array('type'=>BBCODE_TYPE_NOARG, 'open_tag'=>'<b>',
                    'close_tag'=>'</b>', 'content_handling'=>array($foo,'foo_cb')),
    'u'=>        array('type'=>BBCODE_TYPE_NOARG, 'open_tag'=>'<u>',
                    'close_tag'=>'</u>', 'content_handling'=>'callback'),

);
$text=<<<EOF
[i]Italic Text[/i]
[b]Bold Text[/b]
[u]Underlined Text[/u]
EOF;
$BBHandler=bbcode_create($arrayBBCode);
echo bbcode_parse($BBHandler,$text);
?>
--EXPECT--
<i>Foo.bar Italic Text</i>
<b>Foo.foo Bold Text</b>
<u>Callback Underlined Text</u>
