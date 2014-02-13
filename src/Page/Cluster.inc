<?php
/*
 * Uyghur
 * Copyright (c) 2014 Profire Co.,Ltd
 *
 * This software is released under the MIT License.
 *
 * http://opensource.org/licenses/mit-license.php
 */
namespace CassAdmin\Page;

use CassAdmin\Common;
use CassAdmin\Constants;
use CassAdmin\Cassandra;

//========================================================================
/**
 *
 */
//========================================================================
class Cluster extends PageBase
{
    private $params = array(
        'Main' => array(
            ),
        );
    public  $error;
    private $session;
    
    //========================================================================
    /**
     *
    */
    //========================================================================
    public function __construct($manager)
    {
        parent::__construct($manager,$this->params);
        $this->session = &$_SESSION[__CLASS__];

    }

    //========================================================================
    /**
     *
    */
    //========================================================================
    public function Main(&$assigns) {
        extract($this->params[__FUNCTION__]);

        $cas            = new Cassandra($this->manager,KEY_SPACE);

        $partitioner    = $cas->Manager->describe_partitioner();
        $snitch         = $cas->Manager->describe_snitch();
        $exp            = explode(':',$cas->Manager->conn->server);
        $ip             = $exp[0];
        $port           = $exp[1];
        $ksDefs         = $cas->ksDefs();
        
        $assigns += array(
            'ksDefs'         => $ksDefs,
            'partitioner'    => $partitioner,
            'snitch'         => $snitch,
            'ip'             => $ip,
            'port'           => $port,
            'strategy_class' => Constants::StrategyClass(),
            );
        foreach($this->params[__FUNCTION__] as $key => $value ) $assigns[$key] = $$key;
    }

}
?>
