<?php
require_once('myconfig.php');
$db = MYDB();
$db->select('ID,user_login,user_email');
$query = $db->get('wp_users');
echo "\n";
foreach ($query->result() as $row)
{
    print $row->ID . "\n";
    print $row->user_login . "\n";
    print $row->user_email . "\n";
}
?>
