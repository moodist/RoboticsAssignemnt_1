
(cl:in-package :asdf)

(defsystem "rotate_translate-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "InputType" :depends-on ("_package_InputType"))
    (:file "_package_InputType" :depends-on ("_package"))
    (:file "OutputType" :depends-on ("_package_OutputType"))
    (:file "_package_OutputType" :depends-on ("_package"))
  ))