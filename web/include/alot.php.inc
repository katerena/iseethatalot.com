<?php

class Alot {

    /**
     * @var Config $config
     */
    private $config;

    var $id;
    var $word;
    var $image;
    var $added;
    var $verified;
    var $curator_rating;
    var $up_votes;
    var $down_votes;
    var $composed_url;
    var $composed_path;
    var $status;
    var $processed;

    public function __construct($config) {
        $this->config = $config;
    }

    function url() {
        return $this->config->root_url() . $this->ordinal() . '/alot/of/' . $this->slug_word();
    }

    static function parse_ordinal($ordinal) {
        if (preg_match('/^(\d+)\w*$/', $ordinal, $match)) {
            return $match[1];
        }
        return NULL;
    }

    function ordinal() {
        return $this->id;
    }

    function slug_word() {
        //Remove anything totally whacky
        $word = preg_replace('/[^\w\s-]/', '', $this->safe_word());
        //Replace space and - with -
        return preg_replace('/[-\s]+/', '-', $word);
    }

    function safe_word() {
        return htmlentities($this->word);
    }

    function asset_url() {
        if (!$this->composed_url) {
            return FALSE;
        }

        $alot_img = $this->composed_url;
        if ($alot_img[0] !== '/' && (strpos($alot_img, 'http') !== 0)) {
            return $this->config->asset_url($alot_img);
        }
        return $alot_img;
    }

    function nice_creation_time() {
        $date = new DateTime($this->added);
        $now = new DateTime();
        $diff = $now->diff($date);
        if ($diff->days < 1) {
            return $date->format("g:ma, M d");
        } else {
            return $date->format("M d");
        }
    }

    function processing_error() {
        return ($this->processed == 1) && ($this->status);
    }
}