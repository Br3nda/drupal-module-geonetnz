<?php

function georippr_import_page() {
  error_log(__FUNCTION__);

  $batch = array(
    'title' => t('Importing Earthquakes'),
    'operations' => array(
      array('georippr_import_batch', array()),
    ),
    'finished' => 'georippr_batch_finished',
    'file' => 'path_to_file_containing_myfunctions',
  );
  batch_set($batch);
  // only needed if not inside a form _submit handler :
  batch_process();
}

function georippr_import_batch(&$context) {
  error_log(__FUNCTION__);
  if (empty($context['sandbox'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['current_day'] = strtotime('now');
    $context['sandbox']['max'] = strtotime('now') - strtotime('1900-01-01');
  }
  $limit = 5;
  $endDate = date('Y-m-d', $context['sandbox']['current_day']);
  $startDate = $endDate - (60*60*24);
  $result = geonetnz_do_search(array('startDate' => $startDate, 'endDate' => $endDate, 'lowerMagnitude' => 6));
  foreach($results as $r) {
    error_log(print_r($r, 1));
    unset($node);
    $node->created = $r->origin->time->value;
    $node->lat = $r->origin->latitude->value;
    $node->long = $r->origin->longitude->value;
    $node->depth = $r->origin->depth->value;
    $node->magnitude = $r->magnitude->mag->value;
    $node->type = 'earthquake';
    
    
    $context['results'][] = $node->nid .' : '. $node->title;
    $context['sandbox']['progress']++;
    $context['message'] = $node->title;
  }
  if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
}

function georippr_batch_finished() {
}