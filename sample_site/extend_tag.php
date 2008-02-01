<?php
header("Cache-Control: no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");
?>
<html>
  <head>
    <title>
      拡張ﾀｸﾞのﾃｽﾄ
    </title>
  </head>
  <body>
    <center>
    <chxj:if lang="xhtml" parse=true>
      改行ﾀｸﾞは変換されているはず。<br>
    </chxj:if><br>
    <chxj:if lang="xhtml" parse=false>
      改行ﾀｸﾞは変換されていないはず。<br>
    </chxj:if><br>
    <chxj:if lang="hdml" parse=true>
      本文はｾﾝﾀﾘﾝｸﾞされているはず。
    </chxj:if><br>
    <chxj:if lang="hdml" parse=false>
      本文はｾﾝﾀﾘﾝｸﾞされないはず。
    </chxj:if><br>
    <chxj:if lang="jhtml" parse=true>
      文字化けしていないはず→���
    </chxj:if><br>
    <chxj:if lang="jhtml" parse=false>
      文字化けしているはず→���
    </chxj:if><br>
    <chxj:if lang="chtml" parse=true>
      2byteコードのはず→&#9436321;
    </chxj:if><br>
    <chxj:if lang="chtml" parse=false>
      参照文字列ののはず→&#9436321;
    </chxj:if><br>
    </center>
    <hr>
    &#63888;<a href="2.php" accesskey="0">戻る</a><br>
    <hr>
    Copyright(C) 2005-2008 Atsushi Konno <br>
    All rights reserved.
  </body>
</html>
