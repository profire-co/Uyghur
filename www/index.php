<?php
/*
 * Uyghur
 * Copyright (c) 2014 Profire Co.,Ltd
 *
 * This software is released under the MIT License.
 *
 * http://opensource.org/licenses/mit-license.php
 */
namespace CassAdmin;

define('BASE_DIR'     ,realpath('../').'/');

define('SRC_DIR'      ,BASE_DIR.'src/');
define('LIB_DIR'      ,BASE_DIR.'src/libs/');
define('CONF_DIR'     ,BASE_DIR.'conf/');
define('TEMPLATE_DIR' ,BASE_DIR.'template/');
define('COMPILE_DIR'  ,BASE_DIR.'template_c/');
define('CSS_DIR'      ,BASE_DIR.'www/css/');

set_include_path(LIB_DIR);
require_once('phpcassa/autoload.php');
require_once('Poult.inc');

//========================================================================
/**
 *
 */
//========================================================================
spl_autoload_register(function ($class) {
    if (strpos($class, __NAMESPACE__) === FALSE) {
        return false;
    }
    $class = str_replace(__NAMESPACE__.'\\','',$class);
    $class = str_replace('\\','/',$class);

    if ( file_exists(SRC_DIR.$class.'.inc')) {
        require_once(SRC_DIR.$class.'.inc');
    }
});

session_start();

$ini    = parse_ini_file(CONF_DIR."hosts.ini", true);
$def    = Common::strict($_SESSION['server'],key($ini));
$server = Common::getQueryString('server',$def);
$host   = Common::strict($ini[$server]['host']);
$port   = Common::strict($ini[$server]['port']);
$_SESSION['server'] = $server;
define('SERVER' , $server);
define('REQUEST_URI' , $_SERVER['REQUEST_URI']);

$Admin = new CassAdmin($host,$port);
$Admin->Main($ini);


?>
