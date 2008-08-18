--TEST--
BBCODE Child Order Independance
--FILE--
<?php
if (!extension_loaded('bbcode')){
        if (!dl('bbcode.so')){
                die ('bbcode not loadable');
        }
}
$arrayBBCode=array(
    ''=>         array('type'=>BBCODE_TYPE_ROOT,  'childs'=>'!i'),
    'i'=>        array('type'=>BBCODE_TYPE_NOARG, 'open_tag'=>'<i>', 'close_tag'=>'</i>', 'childs'=>'b'),
    'b'=>        array('type'=>BBCODE_TYPE_NOARG, 'open_tag'=>'<b>', 'close_tag'=>'</b>'),
);
$text="[b] [i] [b] Child Order Independance [/b] [/i] [/b]";
$BBHandler=bbcode_create($arrayBBCode);
echo bbcode_parse($BBHandler,$text);
?>
--EXPECT-- 
<b> <i> <b> Child Order Independance </b> </i> </b>