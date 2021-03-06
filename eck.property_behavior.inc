<?php

/**
 * This function checks whether a property is implementing any of the supported
 * property behavior functions
 * 
 * @todo I think this function could simplify the code of eck_property_behavior_invoke_plugin and alter
 */
function eck_property_behavior_implements($entity_type, $property, $function_name){
  $properties = $entity_type->properties;
  $info = $entity_type->properties[$property];
  $return = FALSE;
  
  if(array_key_exists('behavior', $info) && !empty($info['behavior'])){
     $behavior = $info['behavior'];
     $plugin = ctools_get_plugins('eck', 'property_behavior', $behavior);
     $function = ctools_plugin_get_function($plugin, $function_name);
     
     if($function){
       $return = $function;
     }
  }
  
  return $return;
}

/**
 * A general function to call specific functions in a property behavior plugin
 */
function eck_property_behavior_invoke_plugin($entity_type, $function_name, $all = array(), 
 $specific = array()){
  
  $properties = $entity_type->properties;
  $returns = array();
  
  foreach($properties as $property => $info){
    //If there is a behavior associated with this property we need to call the appropiate hooks
    if(array_key_exists('behavior', $info) && !empty($info['behavior'])){
      $behavior = $info['behavior'];
     
      $plugin = ctools_get_plugins('eck', 'property_behavior', $behavior);
      
      $function = ctools_plugin_get_function($plugin, $function_name);
  
      if($function){
        if(array_key_exists($property, $specific)){
          $return = $function($property, array_merge($all, $specific[$property]));
        }else{
          $return = $function($property, $all);
        }
        
        if($return){
          $returns[$property] = $return;
        }
      }
    }
  }
  
  return $returns;
}
 
function eck_property_behavior_invoke_plugin_alter($entity_type, $function_name, $var){
  
  $properties = $entity_type->properties;
  $var['entity_type'] = $entity_type->name;
  $return = $var;
  
  foreach($properties as $property => $info){
    //If there is a behavior associated with this property we need to call the appropiate hooks
    if(array_key_exists('behavior', $info) && !empty($info['behavior'])){
      $behavior = $info['behavior'];
     
      $plugin = ctools_get_plugins('eck', 'property_behavior', $behavior);
      
      $function = ctools_plugin_get_function($plugin, $function_name);
  
      if($function){
        $return = $function($property, $return);
      }
    }
  }
  
  return $return;
  
}

function eck_property_behavior_setter(&$data, $name, $value, $langcode, $type, $info){
  $entity = $data;
  $entity_type_name = $entity->entityType();
  $entity_type = EntityType::loadByName($entity_type_name);
  $property = $name;
  
  $function = eck_property_behavior_implements($entity_type, $property, 'setter');
  
  if($function){
    return $function($property, array('entity'=> $entity, 'value' => $value));
  }
}

function eck_property_behavior_getter($data, array $options, $name, $type, $info){
  $entity = $data;
  $entity_type_name = $entity->entityType();
  $entity_type = EntityType::loadByName($entity_type_name);
  $property = $name;
  
  $function = eck_property_behavior_implements($entity_type, $property, 'getter');
  
  if($function){
    return $function($property, array('entity'=> $entity));
  }
}

function eck_property_behavior_validation($value, $info){
  $entity = $info['parent']->value();
  $entity_type_name = $entity->entityType();
  $entity_type = EntityType::loadByName($entity_type_name);
  $property = $info['schema field'];
  
  $function = eck_property_behavior_implements($entity_type, $property, 'validation');
  
  if($function){
    return $function($property, array('value' => $value));
  }
}

