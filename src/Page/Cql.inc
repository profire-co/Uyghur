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

use cassandra\CqlResultType;
use phpcassa\Schema\DataType;


//========================================================================
/**
 *
 */
//========================================================================
class Cql extends PageBase
{
    private $params = array(
        'Main'   => array(
            ),
        'Query'  => array(
            'query' => NULL,
            ),
        'View' => array(
            'mode'      => NULL,
            'dateTime'  => 'raw',
            'pageLimit' => 30,
            'page'      => 1,
            'next'      => NULL,
            'prev'      => NULL,
            ),
        );
    public  $error;
    public  $template_file;
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
        
        if      ( isset($this->session['Error']['query']) ) {
            $query = $this->session['Error']['query'];
        }
        else if ( isset($this->session['Query']['query']) ) {
            $query = $this->session['Query']['query'];
        }

        if ( !isset($query) ||
             strtolower(Cassandra::extractColumnFamilyFromCql($query)) != strtolower(COLUMN_FAMILY) ) {
            $query = 'SELECT * FROM '.COLUMN_FAMILY;
        }
        if ( isset($this->session['Query']['result']) &&
             $this->session['Query']['result']->type == CqlResultType::VOID ) {
            $result = $this->session['Query']['result'];
        }
        else {
            $result = NULL;
        }
        $assigns += array(
            'error'  => Common::strict($this->session['Error']['message']),
            'query'  => $query,
            'result' => $result,
            );
        unset($this->session['Query']);
        unset($this->session['Error']);

        foreach($this->params[__FUNCTION__] as $key => $value ) $assigns[$key] = $$key;
    }

    //========================================================================
    /**
     *
    */
    //========================================================================
    public function Query(&$assigns) {
        extract($this->params[__FUNCTION__]);
        unset($this->session['Error']);
        if ( !isset($this->session['Query']['result']) ) {
            try {
                $cas    = new Cassandra($this->manager,KEY_SPACE);
                $result = $cas->CqlQuery($query,false);
                
                $this->session['Query'] = array(
                    'query'         => $query,
                    'result'        => $result,
                    'column_family' => $cas->ColumnFamily->column_family,
                );

                if ( $result->type == CqlResultType::ROWS ) {
                    $cfName = $this->session['Query']['column_family'];
                    $url = '?view=Cql&func=View&ksName='.KEY_SPACE.'&cfName='.$cfName;
                }
                else{
                    $url = '?view=Cql&ksName='.KEY_SPACE.'&cfName='.COLUMN_FAMILY;
                }
            }
            catch (\Exception $e) {
                $this->session['Error'] = array(
                    'query'   => $query,
                    'message' => $e->getMessage(),
                    );
                $url = '?view=Cql&ksName='.KEY_SPACE.'&cfName='.COLUMN_FAMILY;
            }
        }
        
        header("Location: $url");
    }
    //========================================================================
    /**
     *
    */
    //========================================================================
    public function View(&$assigns) {
        extract($this->params[__FUNCTION__]);
        if ( !isset($this->session['Query']) ) {
            $url = '?view=Cql&ksName='.KEY_SPACE.'&cfName='.COLUMN_FAMILY;
            header("Location: $url");
            exit();
        }

        $cas                = new Cassandra($this->manager,KEY_SPACE,COLUMN_FAMILY);
        $columnMetadataName = $cas->columnMetadataName();
        $columnMetadata     = $cas->columnMetadata();

        
        if ( is_null($mode) ) {
            if ( empty($columnMetadataName) ) {
                $mode = 'kvs';
            }
            else {
                $mode = 'rdbms';
            }
        }

        $casResult  = $this->session['Query']['result']->rows;
        $resultNum  = count($casResult);
        $query      = $this->session['Query']['query'];
        
        $keyList    = array();
        $keyEmpty   = true;
        $columnList = array();
        $rdbmsList  = array();
        $kvsList    = array();
        $kvsEmpty   = true;
        $kvResult   = array();
        $dateColumn = array();

        if ( $dateTime == 'date') {
            foreach($columnMetadata as $column ) {
                $dataType = $cas->extractDataType($column['validation_class']);
                if ( $dataType == DataType::DATE_TYPE ) {
                    array_push($dateColumn,$column['name']);
                }
            }
            
        }
        
        $first      = ($pageLimit * ( $page -1 ))+1;
        $last       = ($pageLimit * ( $page    ));
        $count      = 0;
        $nResult    = array();
        foreach($casResult as $key=>$value ) {
            $count++;
            if ( $count < $first ) continue;
            $nResult[$key] = $value;
            if ( $count == $last ) break;
        }
        $casResult = $nResult;
        foreach($casResult as $key=> $value ) {
            foreach( $value as $column_key => &$column_value ) {
                if (array_search($column_key,$dateColumn) !== FALSE)  {
                    $column_value   = str_replace('d:','',$column_value);
                    $column_value   = str_replace(';','',$column_value);
                    $second         = substr($column_value,0,10);
                    $micro          = substr($column_value,11);
                    $column_value   = date("Y-m-d H:i:s",$second);
                    $column_value  .= '.'.$micro;
                }
            }
            
            array_push($keyList,$key);
            $keyEmpty        = false;
            $kvResult[$key]  = $value;
            $rdbmsList[$key] = array();
            $kvsList[$key]   = array();

        }

        switch($mode) {
          case 'kvs':
            break;
          case 'rdbms':
            $columnList = $columnMetadataName;
            break;
          case 'rdbmsF':
            foreach( $kvResult as $key=> $values ) {
                foreach( $values as $column => $value ) {
                    if ( array_search($column,$columnList) === FALSE ) {
                        array_push($columnList,$column);
                    }
                }
            }
            sort($columnList);
            break;
        }
        
        foreach( $kvResult as $key => $values ) {
            foreach( $values as $column => $value ) {
                if ( array_search($column,$columnList) !== FALSE ) {
                    $rdbmsList[$key][$column] = $value;
                }
                else {
                    $kvsList[$key][$column] = $value;
                    $kvsEmpty      = false;
                }
            }
        }

        $beginCount = (($page-1)*$pageLimit)+1;
        $endCount   = (count($kvsList)==$pageLimit)?$beginCount+$pageLimit-1:$beginCount+count($kvsList)-1;
        
        $assigns += array(
            'keyEmpty'       => $keyEmpty,
            'keyList'        => $keyList,
            'kvsEmpty'       => $kvsEmpty,
            'kvsList'        => $kvsList,
            'rdbmsList'      => $rdbmsList,
            'columnList'     => $columnList,
            'modeCheckboxes' => Constants::CfBrowseViewMode(),
            'beginCount'     => $beginCount,
            'endCount'       => $endCount,
            'check'          => Common::getQueryString('check'),
            'resultNum'      => $resultNum,
            'query'          => $query,
            'dateTimeRadio'  => Constants::CfBrowseViewDateType(),
            'dateColumn'     => $dateColumn,
            );
        
        foreach($this->params[__FUNCTION__] as $key => $value ) $assigns[$key] = $$key;
    }

}
?>
