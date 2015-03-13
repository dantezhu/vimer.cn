<?php
/*=============================================================================
#
#     FileName: test.php
#         Desc: 测试pluf_form
#
#       Author: dantezhu
#        Email: zny2008@gmail.com
#     HomePage: http://www.vimer.cn
#
#      Created: 2011-05-20 00:02:17
#      Version: 0.0.1
#      History:
#               0.0.1 | dantezhu | 2011-05-20 00:02:17 | initialization
#
=============================================================================*/

require_once 'pluf_form_inc.php';

class MyApp_Form_Simple extends Pluf_Form
{
    public function initFields($extra=array())
    {
        $this->fields['name'] = new Pluf_Form_Field_Varchar(
            array('required' => true,
            'label' => __('List name'),
            'initial' => 'Here the name',
            'help_text' => __('For example: "Happy stuff".'),
        ));

        $this->fields['description'] = new Pluf_Form_Field_Varchar(
            array('required' => false,
            'label' => __('Description'),
            'widget' => 'Pluf_Form_Widget_TextareaInput',
            'widget_attrs' => array('rows' => 5,
            'cols' => 75),
        ));

        $this->fields['list'] = new Pluf_Form_Field_Varchar(
            array('required' => true,
            'label' => __('list'),
            'initial' => 1,
            'widget' => 'Pluf_Form_Widget_SelectInput',
            'widget_attrs' => array(
                'choices' => array(
                    array(
                        '所有'=>0,
                        '部分'=>1,
                    )
                ),
            )
        ));
    }
}

$form = new MyApp_Form_Simple(
    array(
        'name'=>'dantezhu',
        'list'=>1
    )
);

echo $form->render_p();
echo "\n";

echo var_export($form->isValid());
echo "\n";

echo $form->field('name')->labelTag();
echo "\n";
echo $form->field('name');
echo "\n";

echo var_export($form->clean(),true);
?>
