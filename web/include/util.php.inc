<?php
include 'include/db.php.inc';

class Config {
    var $config;
    var $db = NULL;
    var $method;
    var $segments;
    var $app_path;
    var $request_url;
    var $request_path;
    var $parameters;

    public function __construct($config, $resource_map) {
        $this->config = $config;
        $this->resource_map = $resource_map;

        $this->app_path = $this->config['main']['app_path'];
        if ($this->app_path && $this->app_path[0] !== '/') {
            $this->app_path = '/' . $this->app_path;
        }

        $this->is_https = !empty($_SERVER['HTTPS']) && $_SERVER['HTTPS'] != 'off';

        $this->method = $_SERVER['REQUEST_METHOD'];
        $this->parse_url();
    }

    function is_post() {
        return $this->method === 'POST';
    }

    function is_get() {
        return $this->method === 'GET';
    }

    private function parse_url() {
        $url = $_SERVER['REQUEST_URI'];

        //Remove the app path
        if ($this->app_path && substr($url, 0, strlen($this->app_path)) == $this->app_path) {
            $url = substr($url, strlen($this->app_path));
        }

        $this->request_url = $url;

        $path = parse_url($url, PHP_URL_PATH);

        $path_len = strlen($path);
        if ($path[$path_len - 1] == '/') { //chop off a trailing slash
            $path = substr($path, 0, $path_len - 1);
        }

        $this->request_path = $path;

        $this->segments = array_filter(explode('/', $path), 'strlen');
    }

    function get($name, $default = NULL) {
        if (isset($_GET[$name])) {
            return $_GET[$name];
        }
        return $default;
    }

    function param($name, $default = NULL) {
        if (isset($this->parameters[$name])) {
            return $this->parameters[$name];
        }

        return $default;
    }

    /**
     * Get a named url segment, with a fallback to the query string.
     * @param $name
     * @param null $default
     * @return null
     */
    function get_param($name, $default = NULL) {
        if (isset($this->parameters[$name])) {
            return $this->parameters[$name];
        }

        return $this->get($name, $default);
    }

    function post($name, $default = NULL) {
        if (isset($_POST[$name])) {
            return $_POST[$name];
        }
        return $default;
    }
    
    function mkdb() {
        if ($this->db === NULL) {
            $this->db = new DB($this, $this->config['db']['dsn'], $this->config['db']['username'], $this->config['db']['password']);
        }
        return $this->db;
    }
    
    function error($code, $message) {
        $home = $this->root_url();
        include 'include/error.php.inc';
        die();
    }
    
    function alot_of_default() {
        return $this->asset_url('img/alots/default.png');
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

    public function http_status($code, $message) {
        header("HTTP/1.0 $code $message");
    }
    
    public function root_url() {
        return ($this->is_https ? 'https' : 'http')
            . '://' . $_SERVER['HTTP_HOST'] . $this->app_path . '/';
    }

    public function asset_url($asset) {
        if (isset($this->resource_map[$asset])) {
            $asset = $this->resource_map[$asset];
        }

        return $this->root_url() . $asset;
    }

    public function route($routes) {
        //Find any named parameters
        $this->parameters = array();

        foreach ($routes as $key => $page) {

            if (preg_match_all('!\(:\w+\)!', $key, $param_matches, PREG_SET_ORDER)) {
                $param_names = array();
                foreach ($param_matches as $match) {
                    //chop off the : and ( characters
                    $param_names[] = substr($match[0], 2, strlen($match[0]) - 3);
                }

                //Prepare for regex conversion by quoting, watching out for the delimiter we'll be using
                $key = preg_quote($key, '!');
                //Replace all short codes with regex groups
                //-- note that the : and \ characters will have been quoted so we need to catch the backslash also
                //-- still, i have NO IDEA WHY WE NEED SO MANY SLASHES
                $key = preg_replace('!\\\\\(\\\:\w+\\\\\)!', '([\w%-]+)', $key);
                //Surround by start/end markers and delimiters
                $key = '!^' . $key . '$!';
                //Check if it matches the request
                if (preg_match($key, $this->request_path, $matches)) {

                    //Get the parameter values
                    for ($i = 0; $i < count($param_names); $i++) {
                        $name = $param_names[$i];
                        $this->parameters[$name] = urldecode($matches[$i + 1]);
                    }

                    return $page;
                }
            } else if ($key === '*' || $key == $this->request_path) {
                return $page;
            }
        }

        return NULL;
    }
}

function read_config($app='../config/app.ini', $map='../config/map.ini') {
    $app = parse_ini_file($app, TRUE);

    if (file_exists($map)) {
        $map = parse_ini_file($map);
    } else {
        $map = array();
    }

    return new Config($app, $map);
}
