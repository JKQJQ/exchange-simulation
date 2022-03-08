;; locale coding
(setq locale-coding-system'utf-8)
(setq grep-command "grep --exclude=\"*\\.svn*\" -nHi -e ")
(setq grep-build-command "grep --include=\"*\\BUILD\" -nHi -e ")
(setq grep-cpp-command "grep --include=\"*\\.cpp\" --include=\"*\.h\" \. -r -nHi -e ")
(setq grep-etest-command "grep --exclude=\"*/test*\" --include=\"*\\.cpp\" --include=\"*\.h\" \. -r -nHi -e ")
(setq grep-py-command "grep --include=\"*\\.py\" \. -r -nHi ")

(prefer-coding-system'utf-8)
(set-keyboard-coding-system'utf-8)
(set-terminal-coding-system'utf-8)
(set-selection-coding-system'utf-8)
(set-clipboard-coding-system 'ctext)
(set-buffer-file-coding-system 'utf-8)

(let ((default-directory "~/.emacs.d/lisp/"))
    (normal-top-level-add-to-load-path '("."))
      (normal-top-level-add-subdirs-to-load-path))

(global-set-key  (kbd "C-x l") 'windmove-left)
(global-set-key  (kbd "C-x r") 'windmove-right)
(global-set-key  (kbd "C-x p") 'windmove-up)
(global-set-key  (kbd "C-x d") 'windmove-down)

;;eshell settings
(defun open-eshell-other-buffer()
  "Open eshell in other buffer"
  (interactive)
  (split-window-vertically)
  (eshell))
(global-set-key [f8] 'open-eshell-other-buffer)
(global-set-key [f7] 'eshell)
;;(global-set-key [f10] 'add-case-to-header-file)
(global-set-key [f9] 'compile)
(global-set-key [f11] 'bookmark-set)
(global-set-key [f12] 'list-bookmarks)

;;buffer list and complate
(require 'ido)
(ido-mode t)

(load-file "~/.emacs.d/lisp/vespa-style.el")
(require 'vespa-style)
(add-hook 'c-mode-common-hook 'vespa-c-mode-hook)

(load-file "~/.emacs.d/lisp/index-engine-tools.el")
(require 'index-engine-tools)

(load-file "~/.emacs.d/lisp/astyle.el")
(require 'astyle-utils)
(global-set-key [f10] 'astyle)
(add-hook 'before-save-hook 'delete-trailing-whitespace)


    ;;c++ related
    (add-hook 'c++-mode-hook
          '(lambda ( )
             (c-toggle-hungry-state)))
    (add-hook 'c++-mode-hook
          '(lambda ( )
             (flyspell-prog-mode)))

    (add-hook 'c++-mode-hook
          '(lambda ( )
             (c-set-style "vespa")))

    (setq auto-mode-alist
         (cons '("\\.h\\'" . c++-mode) auto-mode-alist))

    (add-to-list 'auto-mode-alist
    '("\\.pp\\'" . c++-mode))

    (add-to-list 'auto-mode-alist
    '("\\.js\\'" . java-mode))
    (add-to-list 'auto-mode-alist
    '("\\BUILD\\'" . python-mode))

    ;; (add-to-list 'auto-mode-alist
    ;; '("\\.ll\\'" . c++-mode))

    ;; (add-to-list 'auto-mode-alist
    ;; '("\\.yy\\'" . c++-mode))

    (add-to-list 'auto-mode-alist
    '("\\.proto\\'" . java-mode))

    ;;c-mode related
    (add-hook 'c-mode-hook
          '(lambda ( )
             (c-toggle-hungry-state)))
    (add-hook 'c-mode-hook
          '(lambda ( )
             (flyspell-prog-mode)))

    ;; enable visual feedback on selections
    ;(setq transient-mark-mode t)

    ;; default to better frame titles
    (setq frame-title-format (concat  "%b - emacs@" (system-name)))

    ;; default to unified diffs
    (setq diff-switches "-u")

    ;;close bell
    (setq visible-bell t)

    ;;not insert tab
    (setq-default indent-tabs-mode nil)

    ;;(require 'linum+)
    ;;(global-linum-mode 1)
    ;;(setq linum-format "%d| ")

    ;;show column number
    (column-number-mode t)

    ;; hide menu bar
    (menu-bar-mode -1)

    ;; high light
    (show-paren-mode t)
    (setq show-paren-style 'parentheses)

    ;;transient-mark-mode
    (setq transient-mark-mode t)

    ;;turn on highlighting current line
    ;;(global-hl-line-mode 1)

    ;; for buffer
    (global-set-key (kbd "M-n") 'next-buffer)
    (global-set-key (kbd "M-p") 'previous-buffer)

    ;; not generate ~ temp file
    (setq make-backup-files nil)

    (defun grepcpp(command-args)
      (interactive
       (list (read-from-minibuffer "Run grep *.cpp *.h: "
                       grep-cpp-command
                       nil
                       nil
                       'grep-cpp-history)))
       (grep command-args))

    (defun grepbuild(command-args)
      (interactive
       (list (read-from-minibuffer "Run grep BUILD: "
                       grep-build-command
                       nil
                       nil
                       'grep-cpp-history)))
       (grep command-args))

    (defun greppy(command-args)
      (interactive
       (list (read-from-minibuffer "Run grep *.py: "
                       grep-py-command
                       nil
                       nil
                       'grep-py-history)))
       (grep command-args))

    (defun grepet(command-args)
      (interactive
       (list (read-from-minibuffer "Run grep ex test *.cpp *.h: "
                       grep-etest-command
                       nil
                       nil
                       'grep-etest-history)))
       (grep command-args))


    ;; ****for compile
    (global-set-key (kbd "C-x C-m") 'compile)
    (setq compile-command "bazel build ... --sandbox_base=/mnt/ram")
    ;;compilation settings
    (defun my-compile()
      "Save buffers and start compile"
      (interactive)
      (save-some-buffers t)
      (switch-to-buffer-other-window "*compilation*")
      (compile compile-command))

    ;;compilation settings
    (defun my-compile2()
      "Save buffers and start compile"
      (interactive)
      (save-some-buffers t)
      ;;(switch-to-buffer-other-window "*compilation*")
      (compile compile-command))

    (defun refresh-buffer()
      "revert buffer without comfirmation"
      (interactive) (revert-buffer t t)
    )

    (global-set-key [f5] 'refresh-buffer)
    (global-set-key [f6] 'my-compile)
    (global-set-key [f7] 'eshell)

    ;;for auto-complete
    ;; (add-to-list 'load-path "~/.emacs.d/")
    ;; (require 'auto-complete-config)
    ;; (add-to-list 'ac-dictionary-directories "~/.emacs.d//ac-dict")
    ;; (ac-config-default)
    ;;(ac-set-trigger-key "TAB")
    ;;(setq ac-auto-start nil)
    ;;(setq ac-auto-show-menu nil)

    ;; for c mode dev
    (setq c-basic-offset 4)
    (defun wcy-c-open-other-file ()
      "if current file is a header file, then open the
       corresponding source file or vice versa.
      "
        (interactive)
        (let ((f (buffer-file-name))
              (headers '("h" "hpp" "hxx"))
              (sources '("c" "cxx" "cpp" "cc")))
          (if f
              (let* ((b (file-name-sans-extension f))
                     (x (file-name-extension f))
                     (s (cond
                         ((member x headers) sources)
                         ((member x sources) headers)
                         (t nil)))
                     (return-value nil))
                (while s
                  (let ((try-file (concat b "." (car s))))
                    (cond
                     ((find-buffer-visiting try-file)
                      (switch-to-buffer (find-buffer-visiting
                                         try-file))
                      (setq s nil
                            return-value t))
                     ((file-readable-p try-file)
                      (find-file try-file)
                      (setq s nil
                            return-value t))
                     (t (setq s (cdr s))))))
                return-value))))

    (global-set-key (kbd "C-x t") 'wcy-c-open-other-file)

    (require 'view-mode-settings)

    ;; Force gdb-mi to not dedicate any windows
    (defadvice gdb-display-buffer (after undedicate-gdb-display-buffer)
      (set-window-dedicated-p ad-return-value nil))
    (ad-activate 'gdb-display-buffer)

    (defadvice gdb-set-window-buffer (after undedicate-gdb-set-window-buffer (name &optional ignore-dedi window))5B
      (set-window-dedicated-p window nil))
    (ad-activate 'gdb-set-window-buffer)

    (require 'package) ;; You might already have this line
    (add-to-list 'package-archives
                 '("melpa" . "http://melpa.org/packages/") t)
    (when (< emacs-major-version 24)
      ;; For important compatibility libraries like cl-lib
      (add-to-list 'package-archives '("gnu" . "http://elpa.gnu.org/packages/")))

    (setq package-enable-at-startup nil)
    (package-initialize)

    (require 'ggtags)
    (add-hook 'c-mode-hook 'ggtags-mode)
    (add-hook 'c++-mode-hook 'ggtags-mode)
    (add-hook 'asm-mode-hook 'ggtags-mode)
    (add-hook 'python-mode-hook 'ggtags-mode)

    (define-key ggtags-navigation-map (kbd "M-<") nil)
    (define-key ggtags-navigation-map (kbd "M->") nil)

    ;; (require 'multi-web-mode)
    ;; (setq mweb-default-major-mode 'html-mode)
    ;; (setq mweb-tags '((php-mode "<\\?php\\|<\\? \\|<\\?=" "\\?>")
    ;;                   (js-mode "<script +\\(type=\"text/javascript\"\\|language=\"javascript\"\\)[^>]*>" "</script>")
    ;;                   (css-mode "<style +type=\"text/css\"[^>]*>" "</style>")))
    ;; (setq mweb-filename-extensions '("php" "htm" "html" "ctp" "phtml" "php4" "php5"))
    ;; (multi-web-global-mode 1)
    (add-hook 'html-mode-hook 'web-mode)
    (setq-default indent-tabs-mode nil)
    (setq web-mode-markup-indent-offset 2)
    (setq web-mode-css-indent-offset 2)
    (setq web-mode-code-indent-offset 2)
    ;; Enable helm-gtags-mode
    ;; (add-hook 'c-mode-hook 'helm-gtags-mode)
    ;; (add-hook 'c++-mode-hook 'helm-gtags-mode)
    ;; (add-hook 'asm-mode-hook 'helm-gtags-mode)
    ;; (add-hook 'python-mode-hook 'helm-gtags-mode)

    ;; Set key bindings
    ;; (eval-after-load "helm-gtags"
    ;;   '(progn
    ;;      (define-key helm-gtags-mode-map (kbd "M-t") 'helm-gtags-find-tag)
    ;;      (define-key helm-gtags-mode-map (kbd "M-r") 'helm-gtags-find-rtag)
    ;;      (define-key helm-gtags-mode-map (kbd "M-s") 'helm-gtags-find-symbol)
    ;;      (define-key helm-gtags-mode-map (kbd "M-g M-p") 'helm-gtags-parse-file)
    ;;      (define-key helm-gtags-mode-map (kbd "C-c <") 'helm-gtags-previous-history)
    ;;      (define-key helm-gtags-mode-map (kbd "C-c >") 'helm-gtags-next-history)
    ;;      (define-key helm-gtags-mode-map (kbd "M-,") 'helm-gtags-pop-stack)))

    ;; (require 'helm-config)

    ;; The default "C-x c" is quite close to "C-x C-c", which quits Emacs.
    ;; Changed to "C-c h". Note: We must set "C-c h" globally, because we
    ;; cannot change `helm-command-prefix-key' once `helm-config' is loaded.
    ;; (global-set-key (kbd "C-c h") 'helm-command-prefix)
    ;; (global-unset-key (kbd "C-x c"))

    ;; (global-set-key (kbd "M-x") 'helm-M-x)
    ;; (global-set-key (kbd "M-y") 'helm-show-kill-ring)
    ;; (global-set-key (kbd "C-x b") 'helm-mini)
    ;; (global-set-key (kbd "C-x C-f") 'helm-find-files)
    ;; (global-set-key (kbd "C-c h o") 'helm-occur)

    (when (executable-find "ack-grep")
      (setq helm-grep-default-command "ack-grep -Hn --no-group --no-color %e %p %f"
            helm-grep-default-recurse-command "ack-grep -H --no-group --no-color %e %p %f"))


    ;; (define-key helm-command-map (kbd "<tab>") 'helm-execute-persistent-action) ; rebind tab to run persistent action
    ;; (define-key helm-command-map (kbd "C-i") 'helm-execute-persistent-action) ; make TAB works in terminal
    ;; (define-key helm-command-map (kbd "C-z")  'helm-select-action) ; list actions using C-z


    ;; (when (executable-find "curl")
    ;;   (setq helm-google-suggest-use-curl-p t))

    ;; (setq helm-quick-update                     t ; do not display invisible candidates
    ;;       helm-split-window-in-side-p           t ; open helm buffer inside current window, not occupy whole other window
    ;;       helm-buffers-fuzzy-matching           t ; fuzzy matching buffer names when non--nil
    ;;       helm-move-to-line-cycle-in-source     t ; move to end or beginning of source when reaching top or bottom of source.
    ;;       helm-ff-search-library-in-sexp        t ; search for library in `require' and `declare-function' sexp.
    ;;       helm-scroll-amount                    8 ; scroll 8 lines other window using M-<next>/M-<prior>
    ;;       helm-ff-file-name-history-use-recentf t)

    ;; (helm-mode 1)

    ;; (require 'ac-helm)  ;; Not necessary if using ELPA package
    ;; (global-set-key (kbd "C-;") 'ac-complete-with-helm)
    ;; (define-key ac-complete-mode-map (kbd "C-;") 'ac-complete-with-helm)

    ;; (add-hook 'after-init-hook 'global-company-mode)

    ;; (require 'company)
    ;; (setq company-idle-delay 0)
    ;; (add-to-list 'company-backends 'company-c-headers)
    ;; (require 'company-c-headers)
    ;; (add-to-list 'company-c-headers-path-system "/usr/include/c++/4.1.2/")

    (custom-set-variables
     ;; custom-set-variables was added by Custom.
     ;; If you edit it by hand, you could mess it up, so be careful.
     ;; Your init file should contain only one such instance.
     ;; If there is more than one, they won't work right.
     )

    ;; (require 'cc-mode)
    ;; (require 'semantic)

    ;; (global-semanticdb-minor-mode 1)
    ;; (global-semantic-idle-scheduler-mode 1)

    ;; (semantic-mode 1)

    ;; (require 'function-args)
    ;; (fa-config-default)
    ;; (define-key c-mode-map  [(contrl tab)] 'moo-complete)
    ;; (define-key c++-mode-map  [(control tab)] 'moo-complete)
    ;; (define-key c-mode-map (kbd "M-o")  'fa-show)
    ;; (define-key c++-mode-map (kbd "M-o")  'fa-show)

    (setenv "PAGER" "/bin/cat")

    (defun call-magit-status()
      "open magit status"
      (interactive) (call-interactively 'magit-status)
      )
    (global-set-key (kbd "C-x g") 'call-magit-status)

(custom-set-faces
 ;; custom-set-faces was added by Custom.
 ;; If you edit it by hand, you could mess it up, so be careful.
 ;; Your init file should contain only one such instance.
 ;; If there is more than one, they won't work right.
 '(magit-branch-current ((t (:inherit magit-branch-local :box 1))))
 '(magit-diff-added ((t (:foreground "#22aa22"))))
 '(magit-diff-added-highlight ((t (:foreground "#22aa22"))))
 '(magit-diff-base ((t (:background "#ffffcc" :foreground "#aaaa11"))))
 '(magit-diff-base-highlight ((t (:background "#eeeebb" :foreground "#aaaa11"))))
 '(magit-diff-conflict-heading ((t nil)))
 '(magit-diff-context ((t nil)))
 '(magit-diff-context-highlight ((t nil)))
 '(magit-diff-file-heading ((t (:weight bold))))
 '(magit-diff-file-heading-highlight ((t (:inherit magit-section-highlight))))
 '(magit-diff-file-heading-selection ((t (:inherit magit-diff-file-heading-highlight :foreground "salmon4"))))
 '(magit-diff-hunk-heading ((t nil)))
 '(magit-diff-hunk-heading-highlight ((t nil)))
 '(magit-diff-hunk-heading-selection ((t (:inherit magit-diff-hunk-heading-highlight :foreground "salmon4"))))
 '(magit-diff-lines-boundary ((t (:inherit magit-diff-lines-heading))))
 '(magit-diff-lines-heading ((t (:inherit magit-diff-hunk-heading-highlight :background "LightSalmon3"))))
 '(magit-diff-our ((t (:inherit magit-diff-removed))))
 '(magit-diff-our-highlight ((t nil)))
 '(magit-diff-removed ((t (:foreground "#aa2222"))))
 '(magit-diff-removed-highlight ((t (:foreground "#aa2222"))))
 '(magit-diff-their ((t (:inherit magit-diff-added))))
 '(magit-diff-their-highlight ((t (:inherit magit-diff-added-highlight))))
 '(magit-dimmed ((t (:foreground "grey50"))))
 '(magit-hash ((t (:foreground "green"))))
 '(magit-item-highlight ((t (:inherit nil))))
 '(magit-section-highlight ((t nil)))
 '(magit-section-secondary-heading ((t (:foreground "green" :weight normal))))
 '(region ((t (:background "blue" :foreground "green")))))

    (setq make-backup-files nil)
    (add-hook 'after-init-hook #'global-flycheck-mode)

    (require 'flycheck)
    (set-face-attribute 'flycheck-warning nil
                        :foreground "black"
                        :background "orange")

    (set-face-attribute 'flycheck-error nil
                        :foreground "black"
                        :background "red")

    (setq-default flycheck-disabled-checkers '(emacs-lisp-checkdoc))

    ;; =============
    ;; irony mode
    ;; =============
    ;; (add-hook 'c++-mode-hook 'irony-mode)
    ;; (add-hook 'c-mode-hook 'irony-mode)
    ;; ;; =============
    ;; ;; company mode
    ;; ;; =============
    ;; (add-hook 'c++-mode-hook 'company-mode)
    ;; (add-hook 'c-mode-hook 'company-mode)
    ;; ;; replace the `completion-at-point' and `complete-symbol' bindings in
    ;; ;; irony-mode's buffers by irony-mode's function
    ;; (defun my-irony-mode-hook ()
    ;;   (define-key irony-mode-map [remap completion-at-point]
    ;;     'irony-completion-at-point-async)
    ;;   (define-key irony-mode-map [remap complete-symbol]
    ;;     'irony-completion-at-point-async))
    ;; (add-hook 'irony-mode-hook 'my-irony-mode-hook)
    ;; (add-hook 'irony-mode-hook 'irony-cdb-autosetup-compile-options)
    ;; (eval-after-load 'company
    ;;   '(add-to-list 'company-backends 'company-irony))
    ;; ;; (optional) adds CC special commands to `company-begin-commands' in order to
    ;; ;; trigger completion at interesting places, such as after scope operator
    ;; ;;     std::|
    ;; (add-hook 'irony-mode-hook 'company-irony-setup-begin-commands)
    ;; ;; =============
    ;; ;; flycheck-mode
    ;; ;; =============
    ;; (add-hook 'c++-mode-hook 'flycheck-mode)
    ;; (add-hook 'c-mode-hook 'flycheck-mode)
    ;; (eval-after-load 'flycheck
    ;;   '(add-hook 'flycheck-mode-hook #'flycheck-irony-setup))
    ;; ;; =============
    ;; ;; eldoc-mode
    ;; ;; =============
    ;; (add-hook 'irony-mode-hook 'irony-eldoc)
    ;; ;; ==========================================
    ;; ;; (optional) bind TAB for indent-or-complete
    ;; ;; ==========================================
    ;; (defun irony--check-expansion ()
    ;;   (save-excursion
    ;;     (if (looking-at "\\_>") t
    ;;       (backward-char 1)
    ;;       (if (looking-at "\\.") t
    ;;         (backward-char 1)
    ;;         (if (looking-at "->") t nil)))))
    ;; (defun irony--indent-or-complete ()
    ;;   "Indent or Complete"
    ;;   (interactive)
    ;;   (cond ((and (not (use-region-p))
    ;;               (irony--check-expansion))
    ;;          (message "complete")
    ;;          (company-complete-common))
    ;;         (t
    ;;          (message "indent")
    ;;          (call-interactively 'c-indent-line-or-region))))
    ;; (defun irony-mode-keys ()
    ;;   "Modify keymaps used by `irony-mode'."
    ;;   (local-set-key (kbd "TAB") 'irony--indent-or-complete)
    ;;   (local-set-key [tab] 'irony--indent-or-complete))
    ;; (add-hook 'c-mode-common-hook 'irony-mode-keys)

    ;; (defvar my-cpp-other-file-alist
    ;;   '(("\\.cpp\\'" (".h" ".hpp" ".ipp"))
    ;;     ("\\.ipp\\'" (".hpp" ".cpp"))
    ;;     ("\\.hpp\\'" (".ipp" ".cpp"))
    ;;     ("\\.cxx\\'" (".hxx" ".ixx"))
    ;;     ("\\.ixx\\'" (".cxx" ".hxx"))
    ;;     ("\\.hxx\\'" (".ixx" ".cxx"))
    ;;     ("\\.c\\'" (".h"))
    ;;     ("\\.h\\'" (".cpp" "c"))))
    ;; (setq-default ff-other-file-alist 'my-cpp-other-file-alist)
    ;; (add-hook 'c-mode-common-hook (lambda ()
    ;;                                 (define-key c-mode-base-map [(meta o)] 'ff-get-other-file)))
    ;; (setq irony--compile-options
    ;;       '("-std=c++11"
    ;;         "-stdlib=libstdc++"
    ;;         "-I/usr/include/"
    ;;         "-I/usr/include/c++/4.9.2/"
    ;;         "-I/ha3_develop/source_code/_external/usr/include/"
    ;;         "-I/ha3_develop/source_code/_external/usr/local/include/"))

(global-set-key (kbd "M-o h") 'move-beginning-of-line)
(global-set-key (kbd "M-o f") 'move-end-of-line)
