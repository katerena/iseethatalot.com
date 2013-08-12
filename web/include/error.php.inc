<?php
if (!isset($home)) {
    $home = '/';
}

if (!isset($code)) {
    $code = 404;
}

if (!isset($message)) {
    $message = 'alot not found';
}

$error_img = $config->asset_url("img/alots/$code.png");

header("HTTP/1.0 $code $message");
?>
<!DOCTYPE html>
<HTML>
<HEAD>
    <TITLE>I SEE THAT ALOT!</TITLE>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    
    <style>
        body {
          margin: 0;
          font-family: "Helvetica Neue", Helvetica, Arial, sans-serif;
          font-size: 14px;
          line-height: 20px;
          color: #333333;
          background-color: #ffffff;
        }
        a {
          color: #0088cc;
          text-decoration: none;
        }
        a:hover,
        a:focus {
          color: #005580;
          text-decoration: underline;
        }
        h1, h2, h3, h4, h5, h6 {
          margin: 10px 0;
          font-weight: bold;
          line-height: 20px;
          color: inherit;
          text-rendering: optimizelegibility;
        }
        h1, h2, h3 {
          line-height: 40px;
        }
        h2 {
          font-size: 31.5px;
        }
        
        .container {
            width: 600px;
            margin: 60px auto;
            text-align: center;
        }
        
        h1 {
            font-size: 64px;
        }
        
        h2 {
            margin: 0;
        }
    
        img {
            margin: 30px 0;
        }
    </style>
</HEAD>
<BODY>
    <div class="container">
        <h1><?php echo $message?></h1>
        
        <IMG src="<?php echo $error_img?>" />
        
        <h2>Return to <a href="<?php echo $config->root_url()?>">ISEETHATALOT.COM</a></h2>
    </div>
</BODY>