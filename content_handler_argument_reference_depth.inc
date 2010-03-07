<?php

/**
 * @file
 * Provide handler to replace reference with title.
 */
class content_handler_argument_reference_depth extends content_handler_argument_reference {
  function option_definition() {
    $options = parent::option_definition();

    $options['depth'] = array('default' => 0);

    $options['exclude_start'] = array('default' => 0);

    return $options;
  }
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['depth'] = array(
      '#type' => 'weight',
      '#title' => t('Depth'),
      '#default_value' => $this->options['depth'],
      '#description' => t('The depth will match nodes reference nodes in the hierarchy. For example, if you have the node reference "fruit" and a child  node reference "apple", with a depth of 1 (or higher) then filtering for the  node reference "fruit" will get nodes that are tagged with "apple" as well as "fruit". If negative, the reverse is true; searching for "apple" will also pick up nodes tagged with "fruit" if depth is -1 (or lower). This argument is SLOW.'),
    );
    $form['exclude_start'] = array(
      '#type' => 'checkbox',
      '#title' => t('Exclude initial argument'),
      '#default_value' => $this->options['exclude_start'],
      '#description' => t('Include argument.'),
    );
  }

  function query() {
    $this->ensure_my_table();
    $nid = $this->argument;
    
    // will do this as individal quieries and slowly due to lazyness
    $table = $this->definition['table'];
    $field = $this->definition['field'];
    
    if ($this->options['depth'] > 0) {
      $depth = $this->options['depth'];
      $nids = _noderefdepth_find_nid_down($nid, $table, $field, $depth) ;
    }
    elseif ($this->options['depth'] < 0) {
      $depth = $this->options['depth'] * -1;
      $nids = _noderefdepth_find_nid_up($nid, $table, $field, $depth) ;
    }
    
    if (empty($this->options['exclude_start'])) {
      $nids[] = $nid;
    }
    
    if ($nids) {
      $this->query->add_where(0, "$this->table_alias.$this->real_field IN (".db_placeholders($nids).")", $nids);
    } else {
      $this->query->add_where(0, "$this->table_alias.$this->real_field = 0");
    }
  }

}
