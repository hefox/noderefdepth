<?php

/**
 * Implementation of hook_views_data(). 
 */
 
function noderefdepth_views_data_alter(&$data) {
  $data2 = array();
  foreach (content_fields() as $field) {
    $module = $field['module'];
    if ($module != 'nodereference') continue ;
    $result = (array) module_invoke($module, 'field_settings', 'views data', $field);
    drupal_alter('field_settings', $result, 'views data', $field);
    if (empty($result)) {
      $result = content_views_field_views_data($field);
    }
    if (is_array($result)) {
      $data2 = array_merge($data2, $result);
    }
  }
  foreach ($data2 as $table =>$info) {
    foreach ($info as $field => $field_info) {
      
      if (empty($field_info['field']) || empty($field_info['argument'])) {
        continue;
      }
      
      // probably a node reference field. 
      foreach (array('group','title','title short','help') as $copy) {
        $data[$table][$field . '_depth'][$copy] = $field_info[$copy];
      }
      $data[$table][$field . '_depth']['title'] .= t(" with depth");
      $data[$table][$field . '_depth']['argument'] =  $field_info['argument'] ;
      $data[$table][$field . '_depth']['argument']['handler'] = 'content_handler_argument_reference_depth';
      
      $data[$table][$field . '_depth']['filter'] =  $field_info['argument'] ;
      $data[$table][$field . '_depth']['filter']['handler'] = 'content_handler_filter_reference_depth';
    }
  }
}


/**
 * Implementation of hook_views_handlers(). 
*/
 
function noderefdepth_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'noderefdepth') ,
    ),
    'handlers' => array(
      // field handlers
      'content_handler_argument_reference_depth' => array(
        'parent' => 'content_handler_argument_reference',
      ),
      'content_handler_filter_reference_depth' => array(
        'parent' => 'content_handler_filter_many_to_one',
      ),
    )
  );
}
