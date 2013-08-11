<?php
include 'include/db.php.inc';

class Config {
    var $config;
    var $db = NULL;
    
    public function __construct($config) {
        $this->config = $config;
    }
    
    function mkdb() {
        if ($this->db === NULL) {
            $this->db = new DB($this->config['db']['dsn'], $this->config['db']['username'], $this->config['db']['password']);
        }
        return $this->db;
    }
    
    function error($code, $message) {
        $home = $this->root_url();
        include 'include/error.php.inc';
        die();
    }
    
    function alot_of_default() {
        return array($this->config['default']['word'], $this->config['default']['img']);
    }
    
    function ga_code() {
        return $this->config['ga']['code'];
    }
    
    function require_curator() {
        $authenticated = FALSE;
        // Check username and password:
        if (isset($_SERVER['PHP_AUTH_USER']) && isset($_SERVER['PHP_AUTH_PW'])){
         
            $username = $_SERVER['PHP_AUTH_USER'];
            $password = $_SERVER['PHP_AUTH_PW'];
            
            if ($this->config['curate']['username'] === $username &&
                $this->config['curate']['password'] === $password) {
                $authenticated = TRUE;
            }
        }
        
        if (!$authenticated) {
            header('WWW-Authenticate: Basic realm="ISEETHATALOT.COM"');
            header('HTTP/1.0 401 Unauthorized');
            echo 'Authentication Required';
            die();
        }
    }
    
    public function root_url() {
        return 'http://' . 
            $_SERVER['HTTP_HOST'] . '/' . 
            $this->config['main']['app_path'] . '/';
    }
    
    public function alot_url($alot_id) {
        return $this->root_url() . '?id=' . $alot_id;
    }
}

function read_config($file='../config/app.ini') {
    return new Config(parse_ini_file($file, TRUE));
}
