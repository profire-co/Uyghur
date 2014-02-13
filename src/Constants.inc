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

use phpcassa\Schema\StrategyClass;
use phpcassa\Schema\DataType;

//========================================================================
/**
 *
 */
//========================================================================
class Constants
{
    private static $list=array(
        'CfBrowseViewMode'=>array(
            'kvs'    => 'kvs',
            'rdbms'  => 'rdbms(column from metadata)',
            'rdbmsF' => 'rdbms(column from value)',
            ),
        'CfBrowseViewDateType'=>array(
            'raw'    => 'raw data',
            'date'   => 'datetime',
            ),
        'StrategyClass' => array(
            StrategyClass::SIMPLE_STRATEGY               => StrategyClass::SIMPLE_STRATEGY ,
            //StrategyClass::NETWORK_TOPOLOGY_STRATEGY     => StrategyClass::NETWORK_TOPOLOGY_STRATEGY,
            StrategyClass::OLD_NETWORK_TOPOLOGY_STRATEGY => StrategyClass::OLD_NETWORK_TOPOLOGY_STRATEGY,
            ),
        'CfUseDataTypeOptions' => array(
            'comparator_type',
            'default_validation_class',
            'key_validation_class'
            )
        );

    //========================================================================
    /**
     *
     */
    //========================================================================
    public static function __callStatic($name, $arguments)
      {
          if ( isset(self::$list[$name]) ) {
              if ( empty($arguments) ) {
                  return self::$list[$name];
              }
              else if (isset($arguments[0]) && isset(self::$list[$name][$arguments[0]])) {
                  return self::$list[$name][$arguments[0]];
              }
              else if (isset($arguments[1])){
                  return $arguments[1];
              }
          }
          return NULL;
      }


    //========================================================================
    /**
     *
     */
    //========================================================================
    public static function CfDataType($name=NULL)
      {
          $dataType = array();
          foreach( array_keys(DataType::$class_map) as $value ) {
              $dataType[$value] = $value;
          }
          
          if ( is_null($name) ) {
              return $dataType;
          }
          else {
              return Common::strict($dataType[$name]);
          }
      }


}
?>
