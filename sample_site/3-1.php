<?php
header("Cache-Control: no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");
?>
<html>
  <head>
    <title>mod_chxjﾃｽﾄｻｲﾄ</title>
  </head>
  <body>
    <center>
    入力ﾌｫｰﾑのﾃｽﾄ1
    </center>
    <hr>
        <font color="#FF0000">■ﾃｷｽﾄﾎﾞｯｸｽ(istyle=1)</font><br>
        <?php echo $_POST["i1"] ?><br>
        <font color="#FF0000">■ﾃｷｽﾄﾎﾞｯｸｽ(istyle=2)</font><br>
        <?php echo $_POST["i2"] ?><br>
        <font color="#FF0000">■ﾃｷｽﾄﾎﾞｯｸｽ(istyle=3)</font><br>
        <?php echo $_POST["i3"] ?><br>
        <font color="#FF0000">■ﾃｷｽﾄﾎﾞｯｸｽ(istyle=4)</font><br>
        <?php echo $_POST["i4"] ?><br>
        <font color="#FF0000">■ﾃｷｽﾄﾎﾞｯｸｽ(istyle=なし)</font><br>
        <?php echo $_POST["i5"] ?><br>
        <font color="#FF0000">■ﾊﾟｽﾜｰﾄﾞ</font><br>
        <?php echo $_POST["pass"]; ?><br>
        <font color="#FF0000">■ﾗｼﾞｵﾎﾞﾀﾝ</font><br>
        <?php echo $_POST["r1"]; ?><br>
        <font color="#FF0000">■ﾁｪｯｸﾎﾞｯｸｽ</font><br>
        <?php 
          $c1 = $_POST["c1"]; 
          if ($c1) {
            foreach($c1 as $key => $val)
            {
              echo $c1[$key]."をﾁｪｯｸしましたね&#63911;&#63911;<br>";
            }
          }
        ?><br>
        <font color="#FF0000">■ｾﾚｸﾄﾎﾞｯｸｽ</font><br>
        <?php echo $_POST["s1"]; ?><br>
        <font color="#FF0000">■送信ﾎﾞﾀﾝ</font><br>
        <?php echo $_POST["submit"]; ?>
    <br>
    <br>
    &#63879;<a href="3.php" accesskey="1">戻る</a><br>
    &#63888;<a href="1.php" accesskey="0">ﾄｯﾌﾟへ戻る</a><br>
    <hr>
    Copyright(C) 2005 Atsushi Konno <br>
    All rights reserved.
  </body>
</html>
