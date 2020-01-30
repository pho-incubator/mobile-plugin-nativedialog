package com.groupsinc.NativeDialog;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.util.Log;

public class NativeDialogController
{
    protected static final String TAG = "NativeDialogController";
    protected final String IMAGE_PICK_ACTION = "com.groupsinc.NativeDialog.PICK_IMAGE";

    private Activity m_activity;
    private long m_id;

    public NativeDialogController(final Activity activity, final long id) {
        if (activity == null)
            Log.w(TAG, "NativeController requires activity.");
        else
            Log.d(TAG, "Setting activity: " + activity.toString());

        m_activity = activity;
        m_id = id;
    }

    public void alert(final String id, final String title, final String message, final String cancelButtonTitle, final String[] otherButtons, final int style) {
        m_activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                AlertDialog.Builder builder = new AlertDialog.Builder(m_activity);
                builder.setTitle(title);
                builder.setCancelable(false);
                builder.setMessage(message);

                int len = otherButtons.length;
                if (len > 0) {
                    builder.setPositiveButton(otherButtons[0], new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialogInterface, int i) {
                            buttonClicked(m_id, id, 1, otherButtons[0]);
                            dialogInterface.dismiss();
                        }
                    });

                    if (len > 1) {
                        builder.setNeutralButton(otherButtons[1], new DialogInterface.OnClickListener() {
                            @Override
                            public void onClick(DialogInterface dialogInterface, int i) {
                                buttonClicked(m_id, id, 2, otherButtons[1]);
                                dialogInterface.dismiss();
                            }
                        });
                    }
                }

                if (cancelButtonTitle != null) {
                    builder.setNegativeButton(cancelButtonTitle, new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialogInterface, int i) {
                            buttonClicked(m_id, id, 0, cancelButtonTitle);
                            dialogInterface.cancel();
                        }
                    });
                }

                builder.create().show();
            }
        });
    }

    public void actionSheet(final String id, final String title, final String cancelButtonTitle, final String destructiveButtonTitle, final String[] otherButtons) {
        m_activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                AlertDialog.Builder builder = new AlertDialog.Builder(m_activity);
                builder.setTitle(title);
                builder.setCancelable(false);

                int indexOffset = 0;
                int len = otherButtons.length;
                if (cancelButtonTitle != null) {
                    len++;
                    indexOffset++;
                }

                if (destructiveButtonTitle != null) {
                    len++;
                    indexOffset++;
                }

                if (len == 0) {
                    Log.w(TAG, "ActionSheet requires at least 1 button label.");
                    return;
                }

                final int buttonIndexOffset = indexOffset;
                final CharSequence[] sequence = new CharSequence[len];
                len = otherButtons.length;

                int i = 0;
                for (; i < len; i++)
                    sequence[i] = otherButtons[i];

                if (destructiveButtonTitle != null)
                    sequence[i++] = destructiveButtonTitle;

                if (cancelButtonTitle != null)
                    sequence[i] = cancelButtonTitle;

                builder.setItems(sequence, new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialogInterface, int i) {
                        int buttonIndex = i;
                        String buttonLabel = sequence[i].toString();

                        if (cancelButtonTitle != null && buttonLabel.equals(cancelButtonTitle)) {
                            dialogInterface.cancel();
                            return;
                        } else if (destructiveButtonTitle != null && buttonLabel.equals(destructiveButtonTitle)) {
                            buttonIndex = buttonIndexOffset - 1;
                        } else {
                            buttonIndex += buttonIndexOffset;
                        }

                        Log.d(TAG, "Clicked button with index: " + buttonIndex + " label: " + buttonLabel);
                        buttonClicked(m_id, id, buttonIndex, buttonLabel);
                        dialogInterface.dismiss();
                    }
                });

                builder.create().show();
            }
        });
    }

    public void openImagePicker(boolean cameraMode)
    {
        Intent imagePickerIntent = new Intent(m_activity, ImagePickerActivity.class);
        imagePickerIntent.putExtra("id", m_id);
        imagePickerIntent.putExtra("cameraMode", cameraMode);

        m_activity.startActivity(imagePickerIntent);
    }

    public native void buttonClicked(long id, String buttonId, int buttonIndex, String buttonLabel);
}
