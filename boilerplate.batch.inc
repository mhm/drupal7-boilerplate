<?php

/**
 * @file
 * Batch related functions.
 *
 * Created by: Topsitemakers
 * http://www.topsitemakers.com/
 */

/**
 * Sample batch init function.
 *
 * Load and then resave all content on the site.
 */
function boilerplate_batch_init() {

  $batch = array(
    'title' => t('Resaving content.'),
    'operations' => array(),
    'init_message' => t('Process started'),
    'progress_message' => t('Processed @current out of @total.'),
    'error_message' => t('An error occurred during processing'),
    'finished' => 'boilerplate_batch_finished',
    'progressive' => TRUE,
    'file' => drupal_get_path('module', 'boilerplate') . '/boilerplate.batch.inc',
  );

  $nids = db_select('node', 'n')
    ->fields('n', array('nid'))
    ->orderBy('n.nid', 'ASC')
    ->execute()
    ->fetchCol();
  foreach ($nids as $nid) {
    $batch['operations'][] = array('boilerplate_batch_worker', array($nid));
  }

  batch_set($batch);
  batch_process('admin/config');
}

/**
 * Batch worker function.
 *
 * This will be called for each batch item.
 */
function boilerplate_batch_worker($nid, &$context) {
  $node = node_load($nid);
  node_save($node);
  $context['results']['processed']++;
}

/**
 * Batch finish function.
 *
 * This will be called once all batch items are processed.
 */
function boilerplate_batch_finished($success, $results, $operations) {
  if ($success) {
    $message = format_plural($results['processed'], 'One node processed.', '@count nodes processed.');
  }
  else {
    $message = t('An error occurred. Check database log for more details.');
  }
  drupal_set_message($message);
}
