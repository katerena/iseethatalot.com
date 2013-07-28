<?php

class DB {

    var $conn;
    
    //$conn = new PDO('mysql:host=localhost;dbname=kukmbr_alot', 'kukmbr_alot', 'kukmbr_alot1');
    public function __construct($dsn, $username=NULL, $password=NULL) {
        try {
            $this->conn = new PDO($dsn, $username, $password, array(
                PDO::MYSQL_ATTR_INIT_COMMAND => 'SET NAMES utf8',
            ));            
        } catch (PDOException $e) {
            trigger_error('Connection failed: ' . $e->getMessage(), E_USER_ERROR);
        }
    }

    public function insert_alot($word, $image, $alot_img) {
        $stmt = $this->conn->prepare("INSERT INTO alot (word,image,alot_img) VALUES (:word,:image,:alot_img)");
        if (!$stmt) {
            trigger_error('Unable to prepare insert_alot', E_USER_ERROR);
            return FALSE;
        }
        
        if (!$stmt->execute(array(':word'  => $word, ':image' => $image, ':alot_img' => $alot_img))) {
            return FALSE;
        }
        
        return $this->conn->lastInsertId();
    }
    
    public function get_alot($id) {
        $stmt = $this->conn->prepare("SELECT word, image, alot_img FROM alot WHERE id=:id");
        if (!$stmt) {
            trigger_error('Unable to prepare get_alot', E_USER_ERROR);
            return FALSE;
        }
        
        if (!$stmt->execute(array(':id' => $id))) {
            $err = $stmt->errorInfo();
            trigger_error($err[2], E_USER_WARNING);
            return FALSE;
        }
        
        return $stmt->fetch();
    }
    
    public function get_best($top_n) {
        //TODO return top-curated alots!
        return array();
    }
    
    public function set_alot_rating($id, $rating) {
        $stmt = $this->conn->prepare('UPDATE alot SET curator_rating=:rating WHERE id=:id');
        if (!$stmt) {
            trigger_error('Unable to prepare set_alot_rating', E_USER_ERROR);
            return FALSE;
        }
        
        return $stmt->execute(array(
            ':rating' => $rating,
            ':id' => $id
        ));
    }
    
    public function get_alots($from=0, $count=100) {
        $stmt = $this->conn->prepare('SELECT * FROM alot ORDER BY added DESC LIMIT :from, :count');
        if (!$stmt) {
            trigger_error('Unable to prepare get_alots', E_USER_ERROR);
            return FALSE;
        }
        
        $stmt->bindParam(':from', $from, PDO::PARAM_INT);
        $stmt->bindParam(':count', $count, PDO::PARAM_INT);
        if (!$stmt->execute()) {
            $err = $stmt->errorInfo();
            trigger_error($err[2], E_USER_WARNING);
            return FALSE;
        }
        
        return $stmt->fetchAll();
    }
    
    public function count_alots() {
        $stmt = $this->conn->query('SELECT COUNT(*) FROM alot');
        if (!$stmt) {
            trigger_error('Unable to count ALL the alots', E_USER_ERROR);
            return FALSE;
        }
        
        $row = $stmt->fetch();
        
        return $row[0];
    }
}

