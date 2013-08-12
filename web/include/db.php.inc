<?php

include_once 'alot.php.inc';

class DB {

    private $config;
    var $conn;
    
    //$conn = new PDO('mysql:host=localhost;dbname=kukmbr_alot', 'kukmbr_alot', 'kukmbr_alot1');
    public function __construct($config, $dsn, $username=NULL, $password=NULL) {
        $this->config = $config;

        try {
            $this->conn = new PDO($dsn, $username, $password, array(
                PDO::MYSQL_ATTR_INIT_COMMAND => 'SET NAMES utf8',
            ));            
        } catch (PDOException $e) {
            trigger_error('Connection failed: ' . $e->getMessage(), E_USER_ERROR);
        }
    }

    public function insert_alot($word, $image, $composed_url) {
        $stmt = $this->conn->prepare("INSERT INTO alot (word,image,composed_url) VALUES (:word,:image,:composed_url)");
        if (!$stmt) {
            trigger_error('Unable to prepare insert_alot', E_USER_ERROR);
            return FALSE;
        }
        
        if (!$stmt->execute(array(':word'  => $word, ':image' => $image, ':composed_url' => $composed_url))) {
            return FALSE;
        }
        
        $id = $this->conn->lastInsertId();
        if ($id) {
            return $this->get_alot($id);
        }
        return FALSE;
    }

    /**
     * @param $id
     * @return bool|Alot
     */
    public function get_alot($id) {
        $stmt = $this->conn->prepare("SELECT * FROM alot WHERE id=:id");
        if (!$stmt) {
            trigger_error('Unable to prepare get_alot', E_USER_ERROR);
            return FALSE;
        }
        
        if (!$stmt->execute(array(':id' => $id))) {
            $err = $stmt->errorInfo();
            trigger_error($err[2], E_USER_WARNING);
            return FALSE;
        }
        
        return $stmt->fetchObject('Alot', array($this->config));
    }

    /**
     * @param int $from
     * @param int $count
     * @return Alot[]|bool
     */
    public function get_best($from=0, $count=5) {
        $stmt = $this->conn->prepare('SELECT * FROM alot
                                        WHERE composed_url IS NOT NULL
                                        ORDER BY curator_rating DESC,
                                            verified DESC
                                        LIMIT :from, :count');
        if (!$stmt) {
            trigger_error('Unable to prepare get_best', E_USER_ERROR);
            return FALSE;
        }

        $stmt->bindParam(':from', $from, PDO::PARAM_INT);
        $stmt->bindParam(':count', $count, PDO::PARAM_INT);

        if (!$stmt->execute()) {
            $err = $stmt->errorInfo();
            trigger_error($err[2], E_USER_WARNING);
            return FALSE;
        }

        return $stmt->fetchAll(PDO::FETCH_CLASS, 'Alot', array($this->config));
    }
    
    public function set_alot_rating($id, $rating) {
        $stmt = $this->conn->prepare('UPDATE alot SET curator_rating=:rating, verified=NOW() WHERE id=:id');
        if (!$stmt) {
            trigger_error('Unable to prepare set_alot_rating', E_USER_ERROR);
            return FALSE;
        }
        
        return $stmt->execute(array(
            ':rating' => $rating,
            ':id' => $id
        ));
    }

    public function vote_for_alot($id, $vote) {
        $stmt = NULL;
        if ($vote == 1) {
            $stmt = $this->conn->prepare('UPDATE alot SET up_votes = up_votes + 1 WHERE id=:id');
        } else if ($vote == -1) {
            $stmt = $this->conn->prepare('UPDATE alot SET down_votes = down_votes + 1 WHERE id=:id');
        }

        if (!$stmt) {
            trigger_error('Unable to prepare vote_for_alot', E_USER_ERROR);
            return FALSE;
        }

        return $stmt->execute(array(
            ':id' => $id
        ));
    }

    public function all_alots($from=0, $count=100) {
        //Alots with no errors and no 0 curator rating
        $stmt = $this->conn->prepare('SELECT * FROM alot
                                        ORDER BY added DESC
                                        LIMIT :from, :count');
        if (!$stmt) {
            trigger_error('Unable to prepare all_alots', E_USER_ERROR);
            return FALSE;
        }

        $stmt->bindParam(':from', $from, PDO::PARAM_INT);
        $stmt->bindParam(':count', $count, PDO::PARAM_INT);

        if (!$stmt->execute()) {
            $err = $stmt->errorInfo();
            trigger_error($err[2], E_USER_WARNING);
            return FALSE;
        }

        return $stmt->fetchAll(PDO::FETCH_CLASS, 'Alot', array($this->config));
    }

    /**
     * @param int $from
     * @param int $count
     * @return Alot[]|bool
     */
    public function get_alots($from=0, $count=5) {
        //Alots with no errors and no 0 curator rating
        $stmt = $this->conn->prepare('SELECT * FROM alot
                                        WHERE composed_url IS NOT NULL
                                        AND (curator_rating IS NULL OR curator_rating != 1)
                                        ORDER BY added DESC
                                        LIMIT :from, :count');
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

        return $stmt->fetchAll(PDO::FETCH_CLASS, 'Alot', array($this->config));
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

