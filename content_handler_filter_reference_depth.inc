<?php

/**
 * Filter handler for node references with depth.
 *
 */
class content_handler_filter_reference_depth  extends content_handler_filter_many_to_one {

  function has_extra_options() { return TRUE; }
  

  function option_definition() {
    $options = parent::option_definition();

    $options['depth'] = array('default' => 0);

    return $options;
  }
  
  function extra_options_form(&$form, &$form_state) {
    parent::extra_options_form($form, $form_state);

    $form['depth'] = array(
      '#type' => 'weight',
      '#title' => t('Depth'),
      '#default_value' => $this->options['depth'],
      '#description' => t('The depth will match nodes tagged with node references in the hierarchy. For example, if you have the node reference "fruit" and a child node reference "apple", with a depth of 1 (or higher) then filtering for the node reference "fruit" will get nodes that are tagged with "apple" as well as "fruit". If negative, the reverse is true; searching for "apple" will also pick up nodes tagged with "fruit" if depth is -1 (or lower).'),
    );
  }

  function query() {
    $this->ensure_my_table();
    // If no filter values are present, then do nothing.
    if (count($this->value) == 0) {
      return;
    } 
    $values = $this->value;
    $nids = array();
    
    $table = $this->definition['table'];
    $field = $this->definition['field'];
    
    foreach ($values as $nid) {

      if ($this->options['depth'] > 0) {
        $depth = $this->options['depth'];
        $nids = array_merge($nids,_noderefdepth_find_nid_down($nid, $table, $field, $depth)) ;
      }
      elseif ($this->options['depth'] < 0) {
        $depth = $this->options['depth'] * -1;
        $nids =array_merge($nids, _noderefdepth_find_nid_up($nid, $table, $field, $depth)) ;
      }
      $nids[] = $nid;
    } 
    
    if ($nids) {
      $this->query->add_where(0, "$this->table_alias.$this->real_field IN (".db_placeholders($nids).")", $nids);
    } else {
      $this->query->add_where(0, "$this->table_alias.$this->real_field = 0");
    }
  }
}
