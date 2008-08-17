--TEST--
BBCODE No parse Test
--FILE--
<?php
if (!extension_loaded("bbcode")){
	if (!dl('bbcode.so')){
		die ('bbcode not loadable');
	}
}
define("____page","");
$text=<<<EOF
[just][center][round][color=red][size=18]Nous Recherchons un(e) ministre d'urgence Contactez [id=7760]moi[/id][/size][/color][/round][/center][/just]<br />
<br />
<br />
<br />
<br />
[just][center][quote=[color=green]Pod Pocket[/color]]<br />
[color=teal]Avec l'arrivé de l'été, arrive le nouveau line 6 Pod Pocket, quelle coinsidence !!!<br />
Il s'agit donc d'un mini simulateur d'ampli de la célèbre ligné Pod qui est le fer de lance de la marque depuis son lancement en 1998.<br />
<br />
Line 6 étant les rois de la simulation d'ampli, la firme s'oriente donc avec ce produit vers le marché de la musique mobile. Cette nouveauté ne sort pas avant les vacances d'été pour rien, il s'avère être très design, facile à transporter et donc l'outil idéal pour les vacances.<br />
<br />
De la taille d'un simple accordeur, le Pod pocket est tout de même ultra complet. En effet, il possède à son menu 32 modélisations d'amplis, 16 baffles et pas moins de 16 effets intégrés. Vous ne rêvez pas, vous vous dites il est si petit mais offre-t-il  les mêmes capacités que le Pod II ? et bien oui !<br />
Et tenez vous bien, il marche à piles (pour se la jouer rebelz sur la plage... :p) ou sur secteur (pour vous perfectionner avant de vous la jouer sur la plage...:p). Donc que vous soyez frimeur à montrer vos 4 accords aux potes en exterieur espérant attirer les demoiselles, ou que vous soyez enfermé dans votre cagibi depuis 2 mois jonglant entre la carte son du pc et votre guitare, il vous est déstiné et vous simplifira certainement les choses.[/color][/quote][/center][/just]<br />
<br />
[just][center][size=16][color=red][b][u]Voici les données constructeur :[/u][/b][/color][/size][/center][/just]<br />
<br />
[just][center][round][color=lightblue][b][u]Caractéristiques :[/u][/b][/color]<br />
[list]<br />
[*]300 presets modifiables<br />
[*]32 modélisations d'amplis<br />
[*]16 modélisations d'enceintes<br />
[*]16 effets<br />
[*]alimentation par un pile AAA<br />
[*]Entrées jack 6.35 et 3.5<br />
[*]Connexion USB<br />
[*]technologie A.I.R. II permettant un enregistrement direct<br />
[*]connexion au site customtone.com pour télécharger jusqu'à 3000 sons<br />
[*]logiciel d'édition PC[/round][/center][/just]<br />
[/list]<br />
<br />
[just][center][round][color=lightgreen][b][u]Modèles d’amplis basés sur:[/u][/b][/color]<br />
[list]<br />
[*]’64 Fender® Deluxe Reverb®<br />
[*]’59 Fender® Bassman®<br />
[*]’68 Marshall® Plexi 100 watt <br />
[*]Rencontre d’un Marshall® JTM-45 et d’un Budda Twinmaster <br />
[*]’60 Tweed Fender® Champ® <br />
[*]Tête Budda Twinmaster<br />
[*]’65 Blackface Fender® Twin Reverb® <br />
[*]’60 Vox® AC 15 <br />
[*]’60 Vox® AC 30 non-Top Boost <br />
[*]’85 Mesa/Boogie® Mark IIc+ canal Clean<br />
[*]’85 Mesa/Boogie® Mark IIc+ canal Drive Canal Clean du Dumble® Overdrive Special<br />
[*]’95 Tête Mesa/Boogie® Dual Rectifier®<br />
[*]’89 Soldano SLO Super Lead Overdrive Canal Drive du Dumble® Overdrive Special <br />
[*]1987 Roland® JC-120 Jazz Chorus Line 6® Insane[/round][/center][/just]<br />
[/list]<br />
[just][center][round][color=lightyellow][b][u]Modèles d’enceintes basés sur:[/u][b][/color]<br />
[list]<br />
[*]1960 Fender® Tweed Champ®<br />
[*]1952 Fender® Tweed Deluxe Reverb® <br />
[*]1960 Vox® AC-15 <br />
[*]1964 Fender® Deluxe Reverb® <br />
[*]1965 Fender® Blackface Twin Reverb®<br />
[*]1967 Vox® AC-30 <br />
[*]1995 Matchless Chieftain <br />
[*]1959 Fender® Bassman® <br />
[*]1996 Marshall® avec haut-parleurs 30 vintage <br />
[*]1978 Marshall® avec haut-parleurs 70 d’origine <br />
[*]1968 Marshall® Basketweave avec Greenbacks Line 6® 4x12 Line 6® 1x12 Line 6® 2x12 Line 6® 4x10 [/round][/center][/just][/list]<br />
<br />
<br />
[just][center][round][color=pink][b][u]16 effets :[/u][/b][/color]<br />
Compresseur ; Délai/Compresseur <br />
Trémolo ; Délai/Trémolo <br />
Chorus 1 ; Délai/Chorus <br />
Chorus 2 ; Délai/Chorus <br />
Flanger 1 ; Délai/Flanger1 <br />
Flanger 2 ; Délai/Flanger<br />
Haut-parleur rotatif<br />
Délai/Délai ; Délai Réverbération.[/round][/center][/just] <br />
<br />
[just][center][round][color=red][b][u]Prix :[/u][/b][/color] <br />
[list]<br />
[*][color=red]99,00€[/color] sur www.woodbrass.com<br />
[*][color=grey]119,00€[/color] prix public conseillé.[/list]<br />
<br />
[color=lime][b][u]Liens :[/u][/b][/color]<br />
<br />
infos tirées des sites suivants :<br />
<br />
[url=http://www.guitariste.com/articles/pocket-pod,890,1.html]www.guitariste.com[/url]<br />
[url=http://fr.line6.com/pocketpod/index.html]www.line6.com[/url]<br />
[/round][/center][/just] <br />
<br />
[just][center][size=16][color=blue][u][border]Questions :[/border][/u][/color][/size]<br />
[list][color=blue]<br />
[*] Pour vous ce type d'appareil est-il indispensable ?<br />
[*] Selon vous c'est une innovation ou une remasterisation ?<br />
[*] Le trouvez vous pratique de par sa taille et ses capacités ?[/color]<br />
[/center][/just][/list]
EOF;
$arrayBBCode=array(
    'h1'=>        array('type'=>BBCODE_TYPE_NOARG,    'flags'=>BBCODE_FLAGS_REMOVE_IF_EMPTY,    'open_tag'=>'<h1>',                                            'close_tag'=>'</h1>'),
    'h2'=>        array('type'=>BBCODE_TYPE_NOARG,    'flags'=>BBCODE_FLAGS_REMOVE_IF_EMPTY,    'open_tag'=>'<h2>',                                            'close_tag'=>'</h2>'),
    'h3'=>        array('type'=>BBCODE_TYPE_NOARG,    'flags'=>BBCODE_FLAGS_REMOVE_IF_EMPTY,    'open_tag'=>'<h3>',                                            'close_tag'=>'</h3>'),
    'b'=>        array('type'=>BBCODE_TYPE_NOARG,    'flags'=>BBCODE_FLAGS_REMOVE_IF_EMPTY,    'open_tag'=>'<b>',                                            'close_tag'=>'</b>'),
    'u'=>        array('type'=>BBCODE_TYPE_NOARG,    'flags'=>BBCODE_FLAGS_REMOVE_IF_EMPTY,    'open_tag'=>'<u>',                                            'close_tag'=>'</u>'),
    'i'=>        array('type'=>BBCODE_TYPE_NOARG,    'flags'=>BBCODE_FLAGS_REMOVE_IF_EMPTY,    'open_tag'=>'<i>',                                            'close_tag'=>'</i>'),
    'm'=>        array('type'=>BBCODE_TYPE_NOARG,    'flags'=>BBCODE_FLAGS_REMOVE_IF_EMPTY,    'open_tag'=>'<a href="/maraboutage.php">',                    'close_tag'=>'</a>'),
    'cap'=>        array('type'=>BBCODE_TYPE_NOARG,    'flags'=>BBCODE_FLAGS_REMOVE_IF_EMPTY,    'open_tag'=>'<span style="text-transform:capitalize;">',    'close_tag'=>'</span>'),
    'mini'=>    array('type'=>BBCODE_TYPE_NOARG,    'flags'=>BBCODE_FLAGS_REMOVE_IF_EMPTY,    'open_tag'=>'<span class="mini">',                            'close_tag'=>'</span>'),
    'right'=>    array('type'=>BBCODE_TYPE_NOARG,    'flags'=>BBCODE_FLAGS_REMOVE_IF_EMPTY,    'open_tag'=>'<div style="text-align:right;">',                'close_tag'=>'</div>'.(basename(____page,".php")=="chat-2"?'&nbsp; &nbsp; &nbsp;':'').''),
    'center'=>    array('type'=>BBCODE_TYPE_NOARG,    'flags'=>BBCODE_FLAGS_REMOVE_IF_EMPTY,    'open_tag'=>'<div style="text-align:center;">',                'close_tag'=>'</div>'.(basename(____page,".php")=="chat-2"?'&nbsp; &nbsp; &nbsp;':'').''),
    'just'=>    array('type'=>BBCODE_TYPE_NOARG,    'flags'=>BBCODE_FLAGS_REMOVE_IF_EMPTY,    'open_tag'=>'<div style="text-align:justify;width:450px;margin:0 auto;">',                'close_tag'=>'</div>'.(basename(____page,".php")=="chat-2"?'&nbsp; &nbsp; &nbsp;':'').''),
    'strike'=>    array('type'=>BBCODE_TYPE_NOARG,    'flags'=>BBCODE_FLAGS_REMOVE_IF_EMPTY,    'open_tag'=>'<span style="text-decoration:line-through;">',    'close_tag'=>'</span>'),
    'ancre'=>    array('type'=>BBCODE_TYPE_NOARG,    'childs'=>'',        'open_tag'=>'<a name="',                                    'close_tag'=>'"> &nbsp;</a>'),
    'moumou'=>    array('type'=>BBCODE_TYPE_NOARG,    'childs'=>'',        'open_tag'=>'<img src="',                                    'close_tag'=>'" />'),
    'round'=>    array('type'=>BBCODE_TYPE_NOARG,    'flags'=>BBCODE_FLAGS_REMOVE_IF_EMPTY,    'open_tag'=>'<div style="border:2px double white;border-radius: 8px; padding:6px; -moz-border-radius: 8px; background-color:#000; color:#FFF;">',    'close_tag'=>'</div>'.(basename(____page,".php")=="chat-2"?'&nbsp; &nbsp; &nbsp;':'').''),
    'hide'=>    array('type'=>BBCODE_TYPE_NOARG,    'childs'=>'',        'open_tag'=>'',                                                'close_tag'=>'',                                            'content_handling'=>'BBVoid'),
    'np'=>        array('type'=>BBCODE_TYPE_NOARG,    'childs'=>'',        'open_tag'=>'',                                                'close_tag'=>''),
    'img'=>        array('type'=>BBCODE_TYPE_NOARG,    'childs'=>'',        'open_tag'=>'<img src="{CONTENT}" alt="',                    'close_tag'=>'" />',            'content_handling'=>'content_validate_url'),
    'swf'=>        array('type'=>BBCODE_TYPE_NOARG,                        'open_tag'=>'<object width="320" height="240"><param name="movie" value="{CONTENT}"></param><param name="wmode" value="transparent"></param><embed src="', 'close_tag'=>'" type="application/x-shockwave-flash" wmode="transparent" width="320" height="240"></embed></object>'),
    'quote'=>    array('type'=>BBCODE_TYPE_OPTARG,    'flags'=>BBCODE_FLAGS_ARG_PARSING|BBCODE_FLAGS_REMOVE_IF_EMPTY,    'open_tag'=>'<fieldset><legend>{PARAM}</legend>',                'close_tag'=>'</fieldset>'.(basename(____page,".php")=="chat-2"?'&nbsp; &nbsp; &nbsp;':'').'',    'default_arg'=>'Citation'),
    'id'=>        array('type'=>BBCODE_TYPE_OPTARG,    'flags'=>BBCODE_FLAGS_REMOVE_IF_EMPTY,    'open_tag'=>'',                                                'close_tag'=>'',            'default_arg'=>'{SELF}',        'content_handling'=>'BBId2User'),
    'msg'=>        array('type'=>BBCODE_TYPE_OPTARG,    'flags'=>BBCODE_FLAGS_REMOVE_IF_EMPTY,    'open_tag'=>'',                                                'close_tag'=>'',            'default_arg'=>'{SELF}',        'content_handling'=>'BBId2Msg'),
    'url'=>        array('type'=>BBCODE_TYPE_OPTARG,    'flags'=>BBCODE_FLAGS_REMOVE_IF_EMPTY,    'open_tag'=>'<a href="{PARAM}">',                                'close_tag'=>'</a>',        'default_arg'=>'{CONTENT}',        'param_handling'=>'validate_url', 'content_handling'=>'content_validate_url'),
    'mail'=>    array('type'=>BBCODE_TYPE_OPTARG,    'flags'=>BBCODE_FLAGS_REMOVE_IF_EMPTY,    'open_tag'=>'<a href="mailto:{PARAM}">',                        'close_tag'=>'</a>',        'default_arg'=>'{CONTENT}'),
    'border'=>    array('type'=>BBCODE_TYPE_OPTARG,    'flags'=>BBCODE_FLAGS_REMOVE_IF_EMPTY,    'open_tag'=>'<div style="border:{PARAM}px solid;'.(basename(____page,".php")=="chat-2"?'margin-left:15px;padding-left:15px;':'').'">',            'close_tag'=>'</div>'.(basename(____page,".php")=="chat-2"?'&nbsp; &nbsp; &nbsp;':'').'',        'default_arg'=>'5'),
    'list'=>    array('type'=>BBCODE_TYPE_OPTARG,    'flags'=>BBCODE_FLAGS_REMOVE_IF_EMPTY,    'childs'=>'*',    'open_tag'=>'',    'close_tag'=>'',    'default_arg'=>'-1',    'content_handling'=>'BBList'),
    '*'=>        array('type'=>BBCODE_TYPE_NOARG,    'flags'=>BBCODE_FLAGS_REMOVE_IF_EMPTY|BBCODE_FLAGS_ONE_OPEN_PER_LEVEL,    'open_tag'=>'<li>',    'close_tag'=>'</li>'),
    'col'=>        array('type'=>BBCODE_TYPE_OPTARG,    'flags'=>BBCODE_FLAGS_REMOVE_IF_EMPTY,    'open_tag'=>'<div style="-moz-column-count:{PARAM}; text-align:justify;-moz-column-gap:8px;'.(basename(____page,".php")=="chat-2"?'margin-left:15px;padding-left:15px;':'').'">',    'close_tag'=>'</div>'.(basename(____page,".php")=="chat-2"?'&nbsp; &nbsp; &nbsp;':'').'',        'default_arg'=>'2'),
    'hexdump'=>    array('type'=>BBCODE_TYPE_OPTARG,    'flags'=>BBCODE_FLAGS_REMOVE_IF_EMPTY,    'childs'=>'',    'open_tag'=>'[Hex Dump]<br />',                                'close_tag'=>''.(basename(____page,".php")=="chat-2"?'&nbsp; &nbsp; &nbsp;':'').'',            'default_arg'=>'0x345622',        'content_handling'=>'BBHexDump'),
    'bcktrc'=>    array('type'=>BBCODE_TYPE_OPTARG,    'flags'=>BBCODE_FLAGS_REMOVE_IF_EMPTY,    'childs'=>'',        'open_tag'=>'',                                                'close_tag'=>''.(basename(____page,".php")=="chat-2"?'&nbsp; &nbsp; &nbsp;':'').'',            'default_arg'=>'0x345622',        'content_handling'=>'BBBackTrace'),
    'bong'=>    array('type'=>BBCODE_TYPE_OPTARG,    'flags'=>BBCODE_FLAGS_REMOVE_IF_EMPTY,    'childs'=>'onglet',    'open_tag'=>'<div id="{PARAM}"'.(basename(____page,".php")=="chat-2"?' style="margin-left:15px;padding-left:15px;"':'').'>',                                'close_tag'=>'</div>'.(basename(____page,".php")=="chat-2"?'&nbsp; &nbsp; &nbsp;':'').'<script type="text/javascript">new OngletsBlock(\'{PARAM}\');</script>','default_arg'=>'Fiche', 'param_handling'=>'random_id'),
    'onglet'=>    array('type'=>BBCODE_TYPE_ARG,        'flags'=>BBCODE_FLAGS_REMOVE_IF_EMPTY,    'parents'=>'bong',    'open_tag'=>'<h4>{PARAM}</h4><div title="{PARAM}" closable="true" style="clear:both;'.(basename(____page,".php")=="chat-2"?'margin-left:15px;padding-left:15px;':'').'">','close_tag'=>'</div>','default_arg'=>'Default'),
    'color'=>    array('type'=>BBCODE_TYPE_ARG,        'flags'=>BBCODE_FLAGS_REMOVE_IF_EMPTY,    'open_tag'=>'<span style="color:{PARAM}">',                    'close_tag'=>'</span>'),
    'size'=>    array('type'=>BBCODE_TYPE_ARG,        'flags'=>BBCODE_FLAGS_REMOVE_IF_EMPTY,    'open_tag'=>'<span style="font-size:{PARAM}px;">',            'close_tag'=>'</span>'),
    'goto'=>    array('type'=>BBCODE_TYPE_ARG,        'flags'=>BBCODE_FLAGS_REMOVE_IF_EMPTY,    'open_tag'=>'<a href="#{PARAM}" onmouseover="this.href=document.location.href.replace(/#.*/img ,\'\')+\'#{PARAM}\'">', 'close_tag'=>'</a>'),
    'fade'=>    array('type'=>BBCODE_TYPE_ARG,        'flags'=>BBCODE_FLAGS_REMOVE_IF_EMPTY,    'childs'=>'',        'open_tag'=>'',                                                'close_tag'=>'',                                            'content_handling'=>'BBDegrade'),
    'wave'=>    array('type'=>BBCODE_TYPE_ARG,        'flags'=>BBCODE_FLAGS_REMOVE_IF_EMPTY,    'childs'=>'',        'open_tag'=>'',                                                'close_tag'=>'',                                            'content_handling'=>'BBWave'),
    'barre'=>    array('type'=>BBCODE_TYPE_SINGLE,                        'open_tag'=>'<hr />'.(basename(____page,".php")=="chat-2"?'&nbsp; &nbsp; &nbsp;':'').''),
);
$arrayBBCode_meta=array(
    'b'=>        array('name'=>'G',            'title'=>'Placer le texte en gras [b][/b]',                            'style'=>'font-weight:bold;'),
    'u'=>        array('name'=>'S',            'title'=>'Souligner le texte [u][/u]',                                'style'=>'text-decoration:underline;'),
    'i'=>        array('name'=>'I',            'title'=>'Placer le texte en italique[i][/i]',                        'style'=>'font-style:italic;'),
    'm'=>        array('name'=>'Marabou',    'title'=>'Faites de la pub pour le marabou![m][/m]'),
    'cap'=>        array('name'=>'Majusc.',    'title'=>'Force le texte à être affiché en capitales [cap][/cap]',    'style'=>'text-transform:capitalize;'),
    'mini'=>    array('name'=>'Mini',        'title'=>'Rendez votre texte tout petit [mini][/mini]',                                                            'class'=>'mini'),
    'right'=>    array('name'=>'Right',        'title'=>'Aligner votre texte à droite [right][/right]',            'style'=>'text-align:right;'),
    'center'=>    array('name'=>'Center',        'title'=>'Centrer votre texte [center][/center]',                    'style'=>'text-align:center;'),
    'just'=>    array('name'=>'Justifié',    'title'=>'Justifier votre texte [just][/just]',                        'style'=>'text-align:justify;'),
    'strike'=>    array('name'=>'Strike',        'title'=>'Barre votre texte [strike] [/strike]',                    'style'=>'text-decoration:line-through;'),
    'ancre'=>    array('name'=>'Ancre',        'title'=>'Placez sur votre fiche un point de repere [ancre][/ancre]'),
    'moumou'=>    'hidden',
    'round'=>    array('name'=>'Round',        'title'=>'Un cadre sur fond noir à bord arrondi à essayer [round][/round]',    'style'=>'border:2px double white;border-radius: 8px; padding:6px; -moz-border-radius: 8px; background-color:#000; color:#FFF;'),
    'hide'=>    array('name'=>'Masquer',    'title'=>'Masquer le contenu (rendre invisible) [hide][/hide]'),
    'np'=>        array('name'=>'Bloc non traité',    'title'=>'Défini un bloc dont le bbcode ne sera pas traité [np][/np]'),
    'quote'=>    array('name'=>'Citation',    'title'=>'Faire une citation [quote]ou [quote=auteur] [/quote]',                        'prompt'=>'Auteur de la citation (Esc si aucun)'),
    'id'=>        array('name'=>'Lien Fiche',    'title'=>'Faire un lien vers la fiche d\'un utilisateur [id=115][/id]',                    'prompt'=>'Id de l\'Ivyen?'),
    'url'=>        array('name'=>'Lien',        'title'=>'Créez un lien à partir d\'une url [url] ou [url=http://...][/url]',            'prompt'=>'Url du lien avec http:// (Esc si url dans la balise)'),
    'img'=>        array('name'=>'Image',        'title'=>'Créez un lien image [img]http://...[/img]',                                    'img_prompt'=>'Url de l\'image avec http:// '),
    'mail'=>    array('name'=>'Lien Mail',    'title'=>'Créez un lien à partir d\'une url [mail] ou [mail=u@c.c][/mail]',                'prompt'=>'Adresse mail (Esc si adresse dans la balise)'),
    'border'=>    array('name'=>'Bordure',    'title'=>'Créez un cadre avec une bordure [border] ou [border=X][/border]',                'prompt'=>'Taille de la bordure (Esc pour defaut)'),
    'list'=>    array('name'=>'Listes',        'title'=>'Créez une liste avec puce [list] ou [list=X][*]element1[*]element2[/list]',    'prompt'=>'Type de liste: (1 / I / a / ...) ( Esc pour défaut )'),
    'col'=>        array('name'=>'Colonnes',    'title'=>'Divise le texte en X colonnes (Firefox 1.5) [col=X][/col]',                    'prompt'=>'Nombre de colonnes'),
    'hexdump'=>    array('name'=>'HexDump',    'title'=>'HexDump (truc me faisant marrer, je suis désolé) [hexdump=0X3456][/hexdump]',    'prompt'=>'Adresse de démarrage format 0xFED3'),
    'bcktrc'=>    array('name'=>'BackTrace',    'title'=>'BackTrace (comme HexDump) [bcktrc=0X3456][/bcktrc]',                            'prompt'=>'Adresse de démarrage format 0xFED3'),
    'msg'=>        array('name'=>'Lien Message','title'=>'Créer un lien direct pour envoyer un message à ivyen [msg=X][/msg]',            'prompt'=>'Id de l\'ivyen'),
    'color'=>    array('name'=>'Couleur',    'title'=>'Défini la couleur du texte [color=XXX][/color]',                                'prompt'=>'Entrez la couleur désirée',            'dropdown'=>array('aqua'=> 'Aqua', 'black'=> 'Noir', 'blue'=> 'Bleu', 'fuchsia'=> 'Fuschia', 'gray'=> 'Gris', 'green'=> 'Vert', 'lime'=> 'Citron Vert', 'maroon'=> 'Marron', 'navy'=> 'Bleu Marine', 'olive'=> 'Olive', 'purple'=> 'Violet', 'red'=> 'Rouge', 'silver'=> 'Argent', 'teal'=> 'Teal', 'white'=> 'Blanc', 'yellow'=> 'Jaune','--'=>'Couleur personnalis&eacute;e', 'def'=>'Choisir couleur')),
    'size'=>    array('name'=>'Size',        'title'=>'Taille du texte [size=XXX][/size]',                                                                                            'dropdown'=>array('24'=> 'Gigantesque (24)', '20'=> 'Enorme (20)', '16'=> 'Grand(16)', '12'=> 'Normal(12)', '8'=> 'Petit(8)', 'def'=>'Taille du texte'),),
    'goto'=>    array('name'=>'Goto',        'title'=>'Crée un lien vers une ancre du meme bloc [goto=X][/goto]',                    'prompt'=>'Ancre à cibler'),
    'fade'=>    array('name'=>'Dégradé',    'title'=>'Fournis des dégradés d\'une couleur à une autre [wave=XX:YY:ZZ:...][/wave] Séparez les couleurs par des : (donnez les couleurs en Hexa ex: 000000:FF0000:00FF00 (ne supporte pas les tags internes)',            'prompt'=>'Donnez les couleurs séparées par un : ex 0000FF:00FF00:FF0000'),
    'wave'=>    array('name'=>'Vagues',        'title'=>'Fait aller en vague un texte [wave=XX:YY][/wave] avec les tailles oscillant entre XX et YY (ne supporte pas les tags internes)',                                                                                'prompt'=>'Donnez le min et le max séparés par un :'),
    'barre'=>    array('name'=>'Barre',         'title'=>'Insere une barre horizontale [barre]'),
);

$BBHandler=bbcode_create($arrayBBCode);
$array_smileys=array(
    "[fight]"    =>'/smileys/smile142.gif',
    ":p"        =>'/smileys/smile019.gif',
    ":-/"        =>'/smileys/smile046.gif',
    "[fuck]"    =>'/smileys/smile007.gif',
    "[y]"        =>'/smileys/smile054.gif',
    "[l]"        =>'/smileys/smile121.gif',
    ":D"        =>'/smileys/smile004.gif',
    ":B"        =>'/smileys/2pointsB.gif',
    ":*"        =>'/smileys/smile013.gif',
    ":))))"        =>'/smileys/smile008.gif',
    ":))"        =>'/smileys/smile005.gif',
    ":)"        =>'/smileys/smile147.gif',
    ";)"        =>'/smileys/smile144.gif',
    ":("        =>'/smileys/smile001.gif',
    ":'("        =>'/smileys/smile002.gif',
    "[6]"        =>'/smileys/smile006.gif',
    ":@"        =>'/smileys/smile003.gif',
    ":o"        =>'/smileys/smile145.gif',
    ":$"        =>'/smileys/smile146.gif',
    ":cret:"    =>'/smileys/lapin.png',
    ":sm:"        =>'/smileys/sm.gif',
    "(8)"        =>'/smileys/note.gif',
    "",
    "[vtff]"    =>'/smileys/smile149.gif',
    "[f]"        =>'/smileys/smile143.gif',
    "[bou]"        =>'/smileys/smile148.gif',
    "[serenade]"=>'/smileys/smile052.gif',
    "[Love]"    =>'/smileys/smile009.gif',
    );
$array_smileys_yahoo=array(
            "yh;))"            =>"/yahoo/yb671.gif",
            "yh;)"            =>"/yahoo/yb63.gif",
            "yh;;)"            =>"/yahoo/yb65.gif",
            "yh:-/"            =>"/yahoo/yb67.gif",
            "yh:x"            =>"/yahoo/yb68.gif",
            "yh:-x"            =>"/yahoo/yb68.gif",
            "yh:\">"        =>"/yahoo/yb69.gif",
            "yh:-P"            =>"/yahoo/yb610.gif",
            "yh:P"            =>"/yahoo/yb610.gif",
            "yh:-*"            =>"/yahoo/yb611.gif",
            "yh:*"            =>"/yahoo/yb611.gif",
            "yh:-O"            =>"/yahoo/yb613.gif",
            "yhX-("            =>"/yahoo/yb614.gif",
            "yh:->"            =>"/yahoo/yb615.gif",
            "yhB-)"            =>"/yahoo/yb616.gif",
            "yh:-S"            =>"/yahoo/yb617.gif",
            "yh>;:)"        =>"/yahoo/yb619.gif",
            "yh>:)"            =>"/yahoo/yb619.gif",
            "yh:(("            =>"/yahoo/yb620.gif",
            "yh:))"            =>"/yahoo/yb621.gif",
            "yh:|"            =>"/yahoo/yb622.gif",
            "yh/:)"            =>"/yahoo/yb623.gif",
            "yhO:-)"        =>"/yahoo/yb625.gif",
            "yh:-B"            =>"/yahoo/yb626.gif",
            "yh=;"            =>"/yahoo/yb627.gif",
            "yhI-)"            =>"/yahoo/yb628.gif",
            "yh|-)"            =>"/yahoo/yb628.gif",
            "yh8-|"            =>"/yahoo/yb629.gif",
            "yh:-$"            =>"/yahoo/yb632.gif",
            "yh[-("            =>"/yahoo/yb633.gif",
            "yh:o)"            =>"/yahoo/yb634.gif",
            "yh8-}"            =>"/yahoo/yb635.gif",
            "yh(:|"            =>"/yahoo/yb637.gif",
            "yh=P~"            =>"/yahoo/yb638.gif",
            "yh:-?"            =>"/yahoo/yb639.gif",
            "yh#-o"            =>"/yahoo/yb640.gif",
            "yh=D>"            =>"/yahoo/yb641.gif",
            "yh:@)"            =>"/yahoo/yb649.gif",
            "yh3:-o"        =>"/yahoo/yb650.gif",
            "yh3:-0"        =>"/yahoo/yb650.gif",
            "yh:(|)"        =>"/yahoo/yb651.gif",
            "yh@};-"        =>"/yahoo/yb653.gif",
            "yh%%-"            =>"/yahoo/yb654.gif",
            "yh**=="        =>"/yahoo/yb655.gif",
            "yh(~~)"        =>"/yahoo/yb656.gif",
            "yh~o)"            =>"/yahoo/yb657.gif",
            "yh*-:)"        =>"/yahoo/yb658.gif",
            "yh8-x"            =>"/yahoo/yb659.gif",
            "yh=:)"            =>"/yahoo/yb660.gif",
            "yh>-)"            =>"/yahoo/yb661.gif",
            "yh:-L"            =>"/yahoo/yb662.gif",
            "yh>):)"        =>"/yahoo/yb648.gif",
            "yh[-o<"        =>"/yahoo/yb663.gif",
            "yh@-) "        =>"/yahoo/yb643.gif",
            "yh$-)"            =>"/yahoo/yb664.gif",
            "yh:-\""        =>"/yahoo/yb665.gif",
            "yh:^O"            =>"/yahoo/yb644.gif",
            "yhb-("            =>"/yahoo/yb666.gif",
            "yh:)>-"        =>"/yahoo/yb667.gif",
            "yh[-X"            =>"/yahoo/yb668.gif",
            "yh\\:D/"        =>"/yahoo/wbyim59.gif",
            "yh>:D<"        =>"/yahoo/yb66.gif",
            "yh=))"            =>"/yahoo/yb624.gif",
            "yhL-)"            =>"/yahoo/yb630.gif",
            "yh:-w"            =>"/yahoo/yb645.gif",
            "yh:-<"            =>"/yahoo/yb646.gif",
            "yh>:P"            =>"/yahoo/yb647.gif",
            "yh>:/"            =>"/yahoo/yb670.gif",
            "yh:-@"            =>"/yahoo/yb676.gif",
            "yh^:)^"        =>"/yahoo/yb677.gif",
            "yh:-j"            =>"/yahoo/yb678.gif",
            "yh(*)"            =>"/yahoo/yb679.gif",
            "yh:D"            =>"/yahoo/yb64.gif",
            "yh:("            =>"/yahoo/yb62.gif",
            "yh:)"            =>"/yahoo/yb61.gif",
            "yh:-&"            =>"/yahoo/yb631.gif",
            );

foreach($array_smileys as $smil=>$replacement){
    $replacement=" <img src=\"http://images.ivynet.org".$replacement."\" alt=\"".($smil)."\" /> ";
    bbcode_add_smiley($BBHandler,$smil,$replacement);
}
foreach($array_smileys_yahoo as $smil=>$replacement){
    $replacement=" <img src=\"http://images.ivynet.org".$replacement."\" alt=\"".($smil)."\" /> ";
    bbcode_add_smiley($BBHandler,$smil,$replacement);
}
bbcode_set_flags($BBHandler,BBCODE_CORRECT_REOPEN_TAGS | BBCODE_ARG_DOUBLE_QUOTE | BBCODE_ARG_SINGLE_QUOTE | BBCODE_ARG_HTML_QUOTE | BBCODE_AUTO_CORRECT, BBCODE_SET_FLAGS_SET);
if (!function_exists('bbCode')){
    function bbCode($message){
        global $BBHandler;
        $err=error_reporting(E_ALL);
        $message=bbcode_parse($BBHandler,$message);
        $message=str_replace("  "," &nbsp;",$message);
        error_reporting($err);
        return $message;
        //$BBHandler->treat($message);
        //return $message;
    }
}

function BBSize($content,$param){
    $param=min(24,max(4,$param));
    return $param;
}
function BBList($content,$param){
    if ($param=="-1"){
        return '<ul>'.$content.'</ul>';
    } else {
        return '<ol type="'.$param.'">'.$content.'</ol>';
    }
}
function BBInt($content,$param){
    $param= (int) $param;
    return $param;
}
function BBBorder($content,$param){
    $param= min(8,max(1,$param));
    return $param;
}
function bbid2user($content,$param){
    if ($param=='{SELF}'){
        $param=(int)$content;
        $content='pseudo';
    }
    $id=$param;
    $content="<a href='".$content."'>".$content."</a>";
    return $content;
}
function BBId2Msg($content,$param){
    if ($param=='{SELF}'){
        $param=(int)$content;
        $content='pseudo';
    }
    $id=$param;
    $content="<a href='/message.php?destinataire=$id'>".html_encode_string($content)."</a>";
    return $content;
}
function BBWave($content,$param="8:24"){
    if ($_GET['page']=="chat-2.php"){
        return ("[wave=$param]".$content."[/wave]");
    }
    $content=str_replace("\r\n","\n",$content);
    $string=preg_split('//u',html_entity_decode(strip_tags($content),ENT_QUOTES,"UTF-8"),-1,PREG_SPLIT_NO_EMPTY);
    list ($min, $max)=explode(":",$param);
    $out="";
    if ($max==$min){
        return true;
    }
    if ($max<$min){
        $tmp=$max;
        $max=$min;
        $min=$tmp;
    }
    if (strlen($content)<$max-$min && $content!=0){
        $step=round(($max-$min)/$content);
    }
    else{
        $step=1;
    }
    $i=$min;
    $up=true;
    foreach ($string as $car){
        $car=html_encode_string($car);
        $out.="<span style='font-size:".$i."px;'>".$car."</span>";
        if ($up){
            $i+=$step;
        }else {
            $i-=$step;
        }
        if ($i>=$max){
            $up=false;
            $i=$max;
        }
        elseif($i<=$min){
            $up=true;
            $i=$min;
        }
    }
    $content=$out;
    return $content;
}
function shortenAuthor($content,$param=""){
    if (strlen($param)>150){
        $param=substr($param,0,150);
    }
    return $param;
}
function BBDegrade($content,$param=""){
    $content=str_replace("\r\n","\n",$content);
    $string=preg_split('//u',html_entity_decode(strip_tags($content),ENT_QUOTES,"UTF-8"),-1,PREG_SPLIT_NO_EMPTY);
    $colors=explode(":",$param);
    $out="";
    foreach($colors as $v){
        $col[]=array('r'=>base_convert(substr($v,0,2),16,10),'g'=>base_convert(substr($v,2,2),16,10),'b'=>base_convert(substr($v,4,6),16,10));
    }
    $str_size=sizeof($string);
    $nb_col=sizeof($col);
    if ($nb_col<2)
    {
        return $content;
    }
    $steps=max(round($str_size/($nb_col-1)),1);
    $last=null;
    foreach ($string as $k=>$car){
        $car=html_encode_string($car);
        if ($k%$steps==0){
            $nb=$k/$steps;
            $r=sprintf("%02x",$col[$nb]['r']);
            $g=sprintf("%02x",$col[$nb]['g']);
            $b=sprintf("%02x",$col[$nb]['b']);
        }
        elseif($k>($steps*($nb_col-1))){
            $nb=$nb_col-1;
            $r=sprintf("%02x",$col[$nb]['r']);
            $g=sprintf("%02x",$col[$nb]['g']);
            $b=sprintf("%02x",$col[$nb]['b']);
        }else{
            $nb=ceil($k/$steps);
            $r=sprintf("%02x",(abs(round($col[$nb-1]['r']-((($col[$nb-1]['r']-$col[$nb]['r'])/$steps)*($k%$steps))))));
            $g=sprintf("%02x",(abs(round($col[$nb-1]['g']-((($col[$nb-1]['g']-$col[$nb]['g'])/$steps)*($k%$steps))))));
            $b=sprintf("%02x",(abs(round($col[$nb-1]['b']-((($col[$nb-1]['b']-$col[$nb]['b'])/$steps)*($k%$steps))))));
        }
        $current=$r.$g.$b;
        if (is_null($last)){
            $first=$current;
        }
        if ($current!=$last){
            $out.='</span><span style="color:#'.$current.';">';
            $last=$current;
        }
        $out.=$car;
    }
    $out.='</span>';
    return substr($out,strlen('</span>'));
}
function BBVoid($content,$param){
    return " ";
}
function BBBackTrace($string,$base)
{
    $string=br2nl($string);
    $base=base_convert($base,16,10);
    $step=16;
    $tmp="";
    $i=0;
    while ($i<strlen($string))
    {
        $tmp.="[<b>0x".base_convert($base+$i,10,16)."-0x".base_convert($base+$i+$step-1,10,16)."</b> : \"".addcslashes(substr($string,$i,$step),"\n\t\\'\"\0")."\"]<br />\n";
        $i+=$step;
    }
    $string=$tmp;
    return $string;
}
function BBHexDump($string,$base)
{
    $string=br2nl($string);
    $base=base_convert($base,16,10);
    $step=16;
    $tmp="";
    $i=0;
    while ($i<strlen($string))
    {
        $tmp.="[<b>0x".base_convert($base+$i,10,16)."-0x".base_convert($base+$i+$step-1,10,16)."</b> : 0x";
        for ($j=0;$j<$step;$j++){
            $tmp.=sprintf("%02x",ord(substr($string,$i+$j,1)));
        }
        $tmp.="]<br />\n";
        $i+=$step;
    }
    $string=$tmp;
    return $string;
}
function random_id($content, $param){
    return uniqid($param."_");
}
function content_validate_url($content,$param){
    if ($param=="{CONTENT}"){
        return validate_url($param,$content);
    }
    return $content;
}
function validate_url($content, $param){
}
//$text="[i] Parser [b] Auto Correction [/i] at work [/b]\n";
bbCode($text);
bbCode($text);
bbCode($text);
bbCode($text);
bbCode($text);
bbCode($text);
bbcode_destroy($BBHandler);
?>
--EXPECT--
Nothing