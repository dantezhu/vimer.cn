<?php
define('BASEPATH', dirname(__FILE__).'/');
define('EXT', '.php');
require_once(BASEPATH . 'database/DB' . EXT);

function &instantiate_class(&$class_object)
{
    return $class_object;
}

function log_message($level = 'error', $message, $php_error = FALSE)
{
    echo($message);
}
function MYDB()
{
    $params = array(
        'dbdriver'  => 'mysql',
        'hostname'  => 'localhost',
        'username'  => 'root',
        'password'  => '',
        'database'  => 'dante',
        'pconnect'  => TRUE,
        'db_debug'  => TRUE,
        'cache_on'  => FALSE,
        'char_set'  => 'utf-8',
        'dbcollat'  => 'utf8_general_ci',
    );
    $db = DB($params,TRUE);    
    return $db;
}
?>
