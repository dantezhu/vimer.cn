<?php
/*=============================================================================
#
#     FileName: pluf_form_inc.php
#         Desc: require pluf_form所有相关文件
#
#       Author: dantezhu
#        Email: zny2008@gmail.com
#     HomePage: http://www.vimer.cn
#
#      Created: 2011-05-20 00:01:52
#      Version: 0.0.1
#      History:
#               0.0.1 | dantezhu | 2011-05-20 00:01:52 | initialization
#
=============================================================================*/

define('PLUF_FORM_BASEPATH', dirname(__FILE__).'/');

require_once PLUF_FORM_BASEPATH.'Form.php';

//safe
require_once PLUF_FORM_BASEPATH.'SafeString.php';

//form
require_once PLUF_FORM_BASEPATH.'Form/Invalid.php';
require_once PLUF_FORM_BASEPATH.'Form/Model.php';

require_once PLUF_FORM_BASEPATH.'Form/Widget.php';
require_once PLUF_FORM_BASEPATH.'Form/Widget/Input.php';
require_once PLUF_FORM_BASEPATH.'Form/Widget/CheckboxInput.php';
require_once PLUF_FORM_BASEPATH.'Form/Widget/DatetimeInput.php';
require_once PLUF_FORM_BASEPATH.'Form/Widget/FileInput.php';
require_once PLUF_FORM_BASEPATH.'Form/Widget/HiddenInput.php';
require_once PLUF_FORM_BASEPATH.'Form/Widget/PasswordInput.php';
require_once PLUF_FORM_BASEPATH.'Form/Widget/ReCaptcha.php';
require_once PLUF_FORM_BASEPATH.'Form/Widget/SelectInput.php';
require_once PLUF_FORM_BASEPATH.'Form/Widget/SelectMultipleInput.php';
require_once PLUF_FORM_BASEPATH.'Form/Widget/TextareaInput.php';
require_once PLUF_FORM_BASEPATH.'Form/Widget/TextInput.php';
require_once PLUF_FORM_BASEPATH.'Form/Widget/TinyMCEInput.php';
require_once PLUF_FORM_BASEPATH.'Form/Widget/SelectMultipleInput/Checkbox.php';

require_once PLUF_FORM_BASEPATH.'Form/Field.php';
require_once PLUF_FORM_BASEPATH.'Form/FieldProxy.php';
require_once PLUF_FORM_BASEPATH.'Form/BoundField.php';
require_once PLUF_FORM_BASEPATH.'Form/Field/Boolean.php';
require_once PLUF_FORM_BASEPATH.'Form/Field/Date.php';
require_once PLUF_FORM_BASEPATH.'Form/Field/Datetime.php';
require_once PLUF_FORM_BASEPATH.'Form/Field/Email.php';
require_once PLUF_FORM_BASEPATH.'Form/Field/File.php';
require_once PLUF_FORM_BASEPATH.'Form/Field/Float.php';
require_once PLUF_FORM_BASEPATH.'Form/Field/Integer.php';
require_once PLUF_FORM_BASEPATH.'Form/Field/ReCaptcha.php';
require_once PLUF_FORM_BASEPATH.'Form/Field/Slug.php';
require_once PLUF_FORM_BASEPATH.'Form/Field/Url.php';
require_once PLUF_FORM_BASEPATH.'Form/Field/Varchar.php';


/**
 * Translate a string.
 *
 * @param string String to be translated.
 * @return string Translated string.
 */
function __($str)
{
    $locale = (isset($GLOBALS['_PX_current_locale'])) ? $GLOBALS['_PX_current_locale'] : 'en';
    if (!empty($GLOBALS['_PX_locale'][$locale][$str][0])) {
        return $GLOBALS['_PX_locale'][$locale][$str][0];
    }
    return $str;
}
?>
